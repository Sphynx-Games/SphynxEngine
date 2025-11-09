#include "spxpch.h"
#include "ActorDeserializer.h"
#include "Serialization/Reader.h"
#include "Scene/Actor.h"

#include "Reflection/PropertyTree.h"
#include "Component/ComponentRegistry.h"
#include "Component/UUIDComponent.h"
#include "Component/NameComponent.h"
#include "Asset/AssetManager.h"


namespace Sphynx
{
	namespace Utils
	{
		extern void ActorCoreTraversal(
			Reflection::PropertyTree& tree,
			const Reflection::Property* property,
			void* data,
			Reflection::IPropertyTreeVisitor& visitor);

		struct ComponentsData
		{
			Actor* Actor;
			Array<const Reflection::Class*> ComponentClasses;
		};
	}

	ActorDeserializer::ActorDeserializer(Actor& actor, Reader&& reader) :
		ActorDeserializer(actor, Reflection::GetClass<Actor>(), std::move(reader))
	{

	}

	ActorDeserializer::ActorDeserializer(Actor& actor, const Reflection::Class& cClass, Reader&& reader) :
		ReflectionDeserializer(&actor, cClass, reader),
		m_Actor(actor),
		m_ComponentsProperty(nullptr)
	{

	}

	ActorDeserializer::~ActorDeserializer()
	{

	}

	void ActorDeserializer::ActorDeserializeTraversal(Reflection::PropertyTree& tree, const Reflection::Property* property, void* data, Reflection::IPropertyTreeVisitor& visitor)
	{
		using namespace Reflection;
		Actor* actor = static_cast<Actor*>(data);

		Utils::ActorCoreTraversal(tree, property, data, visitor);

		// Components
		{
			// set deserializer component property
			ActorDeserializer& deserializer = static_cast<ActorDeserializer&>(visitor);

			const Reflection::Class& cClass = GetClass<Array<std::string>>();
			const Property property{ cClass, "Components", 0 };
			deserializer.m_ComponentsProperty = &property;

			Utils::ComponentsData componentsData{ actor };
			const CommonAttribute::IndexedCollection* collection = cClass.GetAttribute<CommonAttribute::IndexedCollection>();
			visitor.OnBeforeVisitClass(&property, &componentsData, *collection);
			const bool skip = !visitor.VisitClass(&property, &componentsData, *collection);

			// at this point, we should have all components added to the component array in order
			// check what they are and traverse them
			for (size_t i = 0; !skip && i < componentsData.ComponentClasses.Size(); ++i)
			{
				const Class& componentClass = *componentsData.ComponentClasses.Get(i);
				void* component = ComponentRegistry::InvokeGetComponent(componentClass, *actor, false);
				size_t offset = std::distance((std::byte*)component, (std::byte*)data);

				// first lets visit the index of the array
				const std::string indexStr = std::to_string(i);
				const Property fakeProperty{ componentClass, indexStr.c_str(), offset };
				visitor.OnBeforeVisitClass(&fakeProperty, nullptr);
				const bool skip = !visitor.VisitClass(&fakeProperty, nullptr);
				if (!skip) // once visited, now we can keep going
				{
					const Property fakeProperty{ componentClass, componentClass.Name, offset };
					PropertyTree mTree{ fakeProperty.GetType(), component };
					mTree.Traverse(visitor, &fakeProperty);
				}
				visitor.OnAfterVisitClass(&fakeProperty, nullptr);
			}

			visitor.OnAfterVisitClass(&property, actor, *collection);
			deserializer.m_ComponentsProperty = nullptr;
		}
	}

	void ActorDeserializer::ConfigurePropertyTree(Reflection::PropertyTree& tree, const Reflection::Type& type)
	{
		using namespace Reflection;
		auto& params = tree.GetTraversalParams();
		params.CustomTraversal[&type] = &ActorDeserializeTraversal;
	}

	void ActorDeserializer::Deserialize()
	{
		using namespace Reflection;
		PropertyTree::TraversalParams params;
		params.CustomTraversal[&m_Type] = &ActorDeserializeTraversal;
		PropertyTree::Traverse(m_Type, &m_Actor, *this, std::move(params));
	}

	bool ActorDeserializer::VisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer())
		{
			// TODO: this can be done in a generic way via attributes
			// check if it is an asset type
			if (property->GetPointerIndirection() == 1 && AssetManager::IsAssetTypeRegistered({ &property->GetType() }))
			{
				// treat as assethandle
				// TODO: consider inserting property node in tree instead
				uintptr_t& assetPtr = (*(uintptr_t*)data);
				AssetHandle assetHandle{ AssetHandle::Invalid };

				Reflection::Property fakeProperty{ Reflection::GetType<AssetHandle>(), property->Name, 0 };
				Reflection::PropertyTree propertyTree{ fakeProperty.GetType(), &assetHandle };
				propertyTree.Traverse(*this, &fakeProperty);

				if (assetHandle != AssetHandle::Invalid)
				{
					if (auto asset = AssetManager::GetAsset(assetHandle))
					{
						assetPtr = (uintptr_t)asset->GetRawAsset();
					}
				}

				return false;
			}

			return false;
		}

		return ReflectionDeserializer::VisitClass(property, data);
	}

	bool ActorDeserializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (property != m_ComponentsProperty)
		{
			return ReflectionDeserializer::VisitClass(property, data, collection);
		}

		size_t size = GetCollectionCount();
		for (size_t i = 0; i < size; ++i)
		{
			std::string componentName{};
			{
				m_Reader.PushIndex(i);
				m_Reader.PushMap();
				m_Reader.PushKey(0);

				ReflectionDeserializer deserializer{ &componentName, collection.GetValueType(), m_Reader };
				deserializer.Deserialize();

				m_Reader.PopKey();
				m_Reader.PopMap();
				m_Reader.PopIndex();
			}

			auto components = ComponentRegistry::GetComponents();
			auto it = std::find_if(components.begin(), components.end(), [&](const Reflection::Class* cComponent)
				{
					return !strcmp(cComponent->Name, componentName.c_str());
				});

			if (it != components.end())
			{
				Utils::ComponentsData* componentsData = static_cast<Utils::ComponentsData*>(data);
				componentsData->ComponentClasses.Add(*it);
				if (!ComponentRegistry::InvokeHasComponent(**it, *componentsData->Actor))
				{
					ComponentRegistry::InvokeAddComponent(**it, *componentsData->Actor);
				}
			}
			else
			{
				SPX_CORE_ASSERT(false, "Component not found in registry!");
			}
		}

		return true;
	}

	void ActorDeserializer::OnBeforeVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer())
		{
			// TODO: this can be done in a generic way via attributes
			// check if it is an asset type
			if (property->GetPointerIndirection() == 1 && AssetManager::IsAssetTypeRegistered({ &property->GetType() }))
			{
				OnBeforeVisitProperty(property);
			}
		}

		ReflectionDeserializer::OnBeforeVisitClass(property, data);
	}

	void ActorDeserializer::OnAfterVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer())
		{
			// TODO: this can be done in a generic way via attributes
			// check if it is an asset type
			if (property->GetPointerIndirection() == 1 && AssetManager::IsAssetTypeRegistered({ &property->GetType() }))
			{
				OnAfterVisitProperty(property);
			}
		}

		ReflectionDeserializer::OnAfterVisitClass(property, data);
	}
}
