#include "spxpch.h"
#include "EditorSceneDeserializer.h"
#include "EditorScene.h"
#include <Serialization/Reader.h>
#include <Component/Components.h>
#include <Asset/AssetManager.h>
#include <Asset/Prefab/PrefabAsset.h>
/*#include "Scene/Actor.h"
#include "Logging/Log.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"*/


namespace Sphynx
{
	using PrefabComponentContainer = Array<uintptr_t>;

	struct PrefabData
	{
		Actor* actor = nullptr;
		PrefabComponentContainer components;
	};

	namespace Utils
	{
		extern void ActorCoreTraversal(
			Reflection::PropertyTree& tree,
			const Reflection::Property* property,
			void* data,
			Reflection::IPropertyTreeVisitor& visitor);

		void EditorActorDeserializeTraversal(
			Reflection::PropertyTree& tree,
			const Reflection::Property* property,
			void* data,
			Reflection::IPropertyTreeVisitor& visitor)
		{
			using namespace Reflection;
			Actor* actor = static_cast<Actor*>(data);

			EditorScene* scene = static_cast<EditorScene*>(actor->GetScene());
			auto it = std::find_if(scene->GetPrefabActors().begin(), scene->GetPrefabActors().end(), [&](const PrefabActor& prefabActor) { return *actor == prefabActor; });
			if (it == scene->GetPrefabActors().end())
			{
				ActorDeserializeTraversal(tree, property, data, visitor);
				return;
			}

			// Prefab
			ActorCoreTraversal(tree, property, data, visitor);

			// Components
			{
				// set deserializer component property
				EditorSceneDeserializer& deserializer = static_cast<EditorSceneDeserializer&>(visitor);

				PrefabData prefabData;
				prefabData.actor = actor;

				const Reflection::Class& cClass = GetClass<Array<uint32_t>>();
				const Property property{ cClass, "Components", 0 };
				deserializer.SetPrefabComponentsProperty(&property);

				const CommonAttribute::IndexedCollection* collection = cClass.GetAttribute<CommonAttribute::IndexedCollection>();
				visitor.OnBeforeVisitClass(&property, &prefabData, *collection);
				const bool skip = !visitor.VisitClass(&property, &prefabData, *collection);

				for (size_t i = 0; !skip && i < prefabData.components.Size(); ++i)
				{
					const Reflection::Class* componentClass = (const Reflection::Class*)prefabData.components.Get(i);
					if (componentClass == nullptr) continue; // this is a component that is removed

					void* component = ComponentRegistry::InvokeGetComponent(*componentClass, *actor, false);
					size_t offset = std::distance((std::byte*)component, (std::byte*)data);
					const std::string indexStr = std::to_string(i);

					// first lets visit the index of the array
					const Property fakeProperty{ *componentClass, indexStr.c_str(), offset };
					visitor.OnBeforeVisitClass(&fakeProperty, nullptr);
					const bool skip = !visitor.VisitClass(&fakeProperty, nullptr);
					if (!skip) // once visited, now we can keep going
					{
						const Property fakeProperty{ *componentClass, componentClass->Name, offset };
						PropertyTree mTree{ fakeProperty.GetType(), component };
						mTree.Traverse(visitor, &fakeProperty);
					}
					visitor.OnAfterVisitClass(&fakeProperty, nullptr);
				}

				visitor.OnAfterVisitClass(&property, &prefabData, *collection);
				deserializer.SetPrefabComponentsProperty(nullptr);
			}
		}
	}

	EditorSceneDeserializer::EditorSceneDeserializer(EditorScene& scene, Reader&& reader) :
		SceneDeserializer(scene, std::move(reader)),
		m_PrefabEditorComponentsProperty(nullptr)
	{
	}

	bool EditorSceneDeserializer::VisitClass(const Reflection::Property* property, void* data)
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
					assetPtr = (uintptr_t)AssetManager::GetAsset(assetHandle)->GetRawAsset();
				}

				return false;
			}

			return false;
		}

		/*if (data == nullptr)
		{
			m_Writer.PushMap();

			m_Writer.PushKey();
			m_Writer.Write(property->Name);

			m_Writer.PushValue();
			m_Writer.Write((const void*)nullptr, 0ULL);

			m_Writer.PopMap();
			return false;
		}*/

		return SceneDeserializer::VisitClass(property, data);
	}

	bool EditorSceneDeserializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (&collection.GetValueType() != &Reflection::GetClass<Actor>() && property != m_PrefabEditorComponentsProperty)
		{
			return SceneDeserializer::VisitClass(property, data, collection);
		}

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_HasToPopValue.emplace(property);
			size_t size = m_Reader.PushSequence();

			EditorScene* scene = static_cast<EditorScene*>(&m_Scene);
			for (size_t i = 0; i < size; ++i)
			{
				// we need to check if it is a prefab or not
				m_Reader.PushIndex(i);

				if (property == m_PrefabEditorComponentsProperty)
				{
					m_Reader.PushMap();
					m_Reader.PushKey(0);

					std::string componentName{};
					m_Reader.Read(componentName);
					m_Reader.PopKey();

					auto components = ComponentRegistry::GetComponents();
					auto it = std::find_if(components.begin(), components.end(), [&](const Reflection::Class* cComponent)
						{
							return !strcmp(cComponent->Name, componentName.c_str());
						});

					if (it != components.end())
					{
						m_Reader.PushValue(0);
						const bool isNull = m_Reader.IsNull();
						m_Reader.PopValue();

						const Reflection::Class* cComponent = !isNull ? *it : nullptr;
						PrefabData* prefabData = static_cast<PrefabData*>(data);
						prefabData->components.Add((uintptr_t)cComponent);
						
						if (cComponent == nullptr)
						{
							if (ComponentRegistry::InvokeHasComponent(**it, *prefabData->actor, false))
							{
								ComponentRegistry::InvokeRemoveComponent(**it, *prefabData->actor);
							}

						}
						else if (!ComponentRegistry::InvokeHasComponent(*cComponent, *prefabData->actor, false))
						{
							ComponentRegistry::InvokeAddComponent(*cComponent, *prefabData->actor);
						}
					}
					else
					{
						SPX_CORE_ASSERT(false, "Component not found in registry!");
					}

					m_Reader.PopMap();
				}
				else
				{
					m_Reader.PushMap();

					size_t prefabIndex{};
					// we have found a prefab
					if (m_Reader.FindKey("Prefab", prefabIndex))
					{
						AssetHandle prefabHandle = AssetHandle::Invalid;
						const Reflection::Property property{ Reflection::GetType<AssetHandle>(), "Prefab", 0 };
						Reflection::PropertyTree mTree{ property.GetType(), &prefabHandle };
						mTree.Traverse(*this, &property);

						scene->CreatePrefabActor(AssetManager::GetAsset<Prefab>(prefabHandle)->Asset);
					}
					// this is a normal actor
					else
					{
						Actor& actor = scene->CreateActor();
						actor.AddComponent<NameComponent>();
					}

					m_Reader.PopMap();
				}

				m_Reader.PopIndex();
			}
		}

		return true;
	}

	void EditorSceneDeserializer::OnBeforeVisitClass(const Reflection::Property* property, void* data)
	{
		SceneDeserializer::OnBeforeVisitClass(property, data);
	}

	void EditorSceneDeserializer::OnAfterVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;
		SceneDeserializer::OnAfterVisitClass(property, data);
	}

	void EditorSceneDeserializer::Deserialize()
	{
		using namespace Reflection;
		PropertyTree::TraversalParams params;
		params.CustomTraversal[&GetClass<Actor>()] = &Utils::EditorActorDeserializeTraversal;
		PropertyTree::Traverse(GetClass<EditorScene>(), &m_Scene, *this, std::move(params));
	}

	void EditorSceneDeserializer::SetPrefabComponentsProperty(const Reflection::Property* property)
	{
		m_PrefabEditorComponentsProperty = property;
	}

}