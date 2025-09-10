#include "spxpch.h"
#include "SceneSerializer.h"
#include "Serialization/Writer.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Component/Components.h"
#include "Logging/Log.h"
#include "Serialization/Reflection/ReflectionSerializer.h"


namespace Sphynx
{
	namespace Utils
	{
		void ActorCoreTraversal(
			Reflection::PropertyTree& tree,
			const Reflection::Property* property,
			void* data,
			Reflection::IPropertyTreeVisitor& visitor)
		{
			using namespace Reflection;
			Actor* actor = static_cast<Actor*>(data);

			// UUID
			{
				SPX_CORE_ASSERT(actor->HasComponent<UUIDComponent>(), "Actor should have a UUIDComponent");
				const UUIDComponent* uuidComponent = actor->TryGetComponent<UUIDComponent>();

				const size_t offset = std::distance((std::byte*)data, (std::byte*)&uuidComponent->UUID);
				const Property property{ GetType<UUID>(), "UUID", offset };
				PropertyTree mTree{ property.Type, (void*)&uuidComponent->UUID };
				mTree.Traverse(visitor, &property);
			}
			// Name
			{
				SPX_CORE_ASSERT(actor->HasComponent<NameComponent>(), "Actor should have a NameComponent");
				const NameComponent* nameComponent = actor->TryGetComponent<NameComponent>();

				const size_t offset = std::distance((std::byte*)data, (std::byte*)&nameComponent->Name);
				const Property property{ GetType<std::string>(), "Name", offset };
				PropertyTree mTree{ property.Type, (void*)&nameComponent->Name };
				mTree.Traverse(visitor, &property);
			}
		}

		void ActorTraversal(
			Reflection::PropertyTree& tree, 
			const Reflection::Property* property, 
			void* data, 
			Reflection::IPropertyTreeVisitor& visitor)
		{
			using namespace Reflection;
			Actor* actor = static_cast<Actor*>(data);

			ActorCoreTraversal(tree, property, data, visitor);

			// Components
			{
				Array<const Reflection::Class*> actorComponents;
				for (const Reflection::Class* cClass : ComponentRegistry::GetComponents())
				{
					if (cClass == &Reflection::GetClass<UUIDComponent>() ||
						cClass == &Reflection::GetClass<NameComponent>()) continue;

					if (ComponentRegistry::InvokeHasComponent(*cClass, *actor, false))
					{
						actorComponents.Add(cClass);
					}
				}
				const Reflection::Class& cClass = GetClass<Array<uintptr_t>>();
				const Property property{ cClass, "Components", 0 };
				const CommonAttribute::IndexedCollection* collection = cClass.GetAttribute<CommonAttribute::IndexedCollection>();
				visitor.OnBeforeVisitClass(&property, &actorComponents, *collection);
				const bool skip = !visitor.VisitClass(&property, &actorComponents, *collection);

				for (size_t i = 0; !skip && i < actorComponents.Size(); ++i)
				{
					const Class& componentClass = *actorComponents.Get(i);
					void* component = ComponentRegistry::InvokeGetComponent(componentClass, *actor, false);
					size_t offset = std::distance((std::byte*)component, (std::byte*)data);
					const Property fakeProperty{ componentClass, componentClass.Name, offset };
					PropertyTree mTree{ fakeProperty.Type, component };
					mTree.Traverse(visitor, &fakeProperty);
				}

				visitor.OnAfterVisitClass(&property, &actorComponents, *collection);
			}
		}
	}

	SceneSerializer::SceneSerializer(const Scene& scene, Writer&& writer) :
		m_Scene(scene),
		m_Writer(writer)
	{}

	void SceneSerializer::Serialize()
	{
		using namespace Reflection;
		PropertyTree::TraversalParams params;
		params.CustomTraversal[&GetClass<Actor>()] = &Utils::ActorTraversal;
		PropertyTree::Traverse(GetClass<Scene>(), (void*)&m_Scene, *this, std::move(params));
	}

	void SceneSerializer::Visit(const Reflection::Property* property, bool& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, char& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, signed char& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, wchar_t& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, short& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, int& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, long& data)
	{
		m_Writer.Write(property->Name, (int32_t&)data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, long long& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, float& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, double& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, long double& data)
	{
		m_Writer.Write(property->Name, (double&)data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, unsigned char& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, unsigned short& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, unsigned int& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, unsigned long& data)
	{
		m_Writer.Write(property->Name, (uint32_t&)data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, unsigned long long& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, ::std::string& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, ::std::wstring& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::Visit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		m_Writer.Write(property->Name, data);
	}

	void SceneSerializer::VisitEnum(const Reflection::Property* property, void* data)
	{
		const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(property->Type);
		m_Writer.Write(property->Name, rEnum.GetName((const void*)data));
	}

	bool SceneSerializer::VisitClass(const Reflection::Property* property, void* data)
	{
		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(property->Type);
		if (&rClass == &Reflection::GetClass<Actor>())
		{
			m_Writer.PushMap();
			return true;
		}

		m_Writer.PushKey();
		m_Writer.Write(property->Name);

		/*
		using CustomSerializer = ::Sphynx::Serialization::CustomSerializer<TWriter>;
		if (const CustomSerializer* serializer = rClass.GetAttribute<CustomSerializer>())
		{
			serializer->Write(data, m_Writer);
			return false;
		}
		*/

		// TODO: bring back custom serializers
		if (&Reflection::GetClass<UUID>() == &property->Type)
		{
			UUID* uuid = static_cast<UUID*>(data);
			m_Writer.Write(UUID::ToString(*uuid));
			return false;
		}

		if (m_Writer.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			// Treat Plain Old Data as directly copyable
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				m_Writer.Write((const void*)data, rClass.Size);
				return false;
			}
		}

		m_Writer.PushMap();
		return true;
	}

	bool SceneSerializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		m_Writer.PushKey();
		m_Writer.Write(property->Name);
		m_Writer.PushValue();

		size_t collectionSize = collection.GetSize(data);
		if (!collectionSize) m_Writer.PushFlow();
		m_Writer.PushSequence();
		return true;
	}

	bool SceneSerializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		m_Writer.PushKey();
		m_Writer.Write(property->Name);
		m_Writer.PushValue();

		size_t collectionSize = collection.GetSize(data);
		if (collectionSize == 0) return false;

		m_Writer.PushMap();
		return true;
	}

	void SceneSerializer::OnBeforeVisitEnum(const Reflection::Property* property, void* data)
	{

	}

	void SceneSerializer::OnAfterVisitEnum(const Reflection::Property* property, void* data)
	{

	}

	void SceneSerializer::OnBeforeVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->Name == property->Type.Name)
		{
			m_Writer.PushMap();
		}
	}

	void SceneSerializer::OnAfterVisitClass(const Reflection::Property* property, void* data)
	{
		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(property->Type);
		if (&rClass == &Reflection::GetClass<Actor>())
		{
			m_Writer.PopMap();
			return;
		}
		/*
		using CustomSerializer = ::Sphynx::Serialization::CustomSerializer<TWriter>;
		if (const CustomSerializer* serializer = rClass.GetAttribute<CustomSerializer>())
		{
			return;
		}
		*/
		if (&Reflection::GetClass<UUID>() == &property->Type)
		{
			return;
		}

		if (m_Writer.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				return;
			}
		}

		m_Writer.PopMap();

		if (property->Name == property->Type.Name)
		{
			m_Writer.PopMap();
		}
	}

	void SceneSerializer::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{

	}

	void SceneSerializer::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		m_Writer.PopSequence();
	}

	void SceneSerializer::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{

	}

	void SceneSerializer::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		size_t collectionSize = collection.GetSize(data);
		if (collectionSize == 0) return;

		m_Writer.PopMap();
	}

}