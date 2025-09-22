#include "spxpch.h"
#include "SceneDeserializer.h"
#include "Serialization/Reader.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Component/Components.h"
#include "Logging/Log.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"
#include "Core/UUID.h"
#include <charconv>


namespace Sphynx
{
	namespace Utils
	{
		extern void ActorCoreTraversal(
			Reflection::PropertyTree& tree,
			const Reflection::Property* property,
			void* data,
			Reflection::IPropertyTreeVisitor& visitor);

		void ActorDeserializeTraversal(
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
				// set deserializer component property
				SceneDeserializer& deserializer = static_cast<SceneDeserializer&>(visitor);

				const Reflection::Class& cClass = GetClass<Array<std::string>>();
				const Property property{ cClass, "Components", 0 };
				deserializer.SetComponentsProperty(&property);

				const CommonAttribute::IndexedCollection* collection = cClass.GetAttribute<CommonAttribute::IndexedCollection>();
				visitor.OnBeforeVisitClass(&property, actor, *collection);
				const bool skip = !visitor.VisitClass(&property, actor, *collection);

				// at this point, we should have all components added to the actor
				// check what they are and traverse them
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

				for (size_t i = 0; !skip && i < actorComponents.Size(); ++i)
				{
					const Class& componentClass = *actorComponents.Get(i);
					void* component = ComponentRegistry::InvokeGetComponent(componentClass, *actor, false);
					size_t offset = std::distance((std::byte*)component, (std::byte*)data);
					const std::string indexStr = std::to_string(i);

					// first lets visit the index of the array
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
				deserializer.SetComponentsProperty(nullptr);
			}
		}
	}

	SceneDeserializer::SceneDeserializer(Scene& scene, Reader&& reader) :
		m_Scene(scene),
		m_Reader(reader),
		m_ComponentsProperty(nullptr),
		m_HasToPopValue()
	{
	}

	void SceneDeserializer::Deserialize()
	{
		using namespace Reflection;
		PropertyTree::TraversalParams params;
		params.CustomTraversal[&GetClass<Actor>()] = &Utils::ActorDeserializeTraversal;
		PropertyTree::Traverse(GetClass<Scene>(), (void*)&m_Scene, *this, std::move(params));
	}

	void SceneDeserializer::SetComponentsProperty(const Reflection::Property* property)
	{
		m_ComponentsProperty = property;
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, bool& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, char& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, signed char& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, wchar_t& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, short& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, int& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, long& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			int32_t tmpData = 0;
			m_Reader.PushValue(index);
			m_Reader.Read(tmpData);
			m_Reader.PopValue();
			tmpData = data;
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, long long& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, float& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, double& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, long double& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			double tmpData = 0.0f;
			m_Reader.PushValue(index);
			m_Reader.Read(tmpData);
			m_Reader.PopValue();
			data = tmpData;
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, unsigned char& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, unsigned short& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, unsigned int& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, unsigned long& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			uint32_t tmpData = 0;
			m_Reader.PushValue(index);
			m_Reader.Read(tmpData);
			m_Reader.PopValue();
			data = tmpData;
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, unsigned long long& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, ::std::string& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, ::std::wstring& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::Visit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_Reader.Read(data);
			m_Reader.PopValue();
		}

		if (isIndex) m_Reader.PopIndex();
	}

	void SceneDeserializer::VisitEnum(const Reflection::Property* property, void* data)
	{
		size_t indexValue = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), indexValue);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (isIndex) m_Reader.PushIndex(indexValue);

		const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(property->GetType());
		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			std::string tmpData{};
			m_Reader.PushValue(index);
			m_Reader.Read(tmpData);
			m_Reader.PopValue();
			rEnum.SetName(data, tmpData);
		}

		if (isIndex) m_Reader.PopIndex();
	}

	bool SceneDeserializer::VisitClass(const Reflection::Property* property, void* data)
	{
		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(property->GetType());

		int value = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), value);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);

		if (!isIndex)
		{
			size_t index = 0;
			if (m_Reader.FindKey(property->Name, index))
			{
				m_Reader.PushValue(index);
				m_HasToPopValue.emplace(property);
			}
			else
			{
				return false;
			}
		}
		else
		{
			m_Reader.PushIndex(value);
		}

		/*
		using CustomDeserializer = ::Sphynx::Serialization::CustomDeserializer<TReader>;
		if (const CustomDeserializer* deserializer = rClass.GetAttribute<CustomDeserializer>())
		{
			deserializer->Read(data, m_Reader);
			return false;
		}
		*/

		// TODO: bring back custom deserializers
		if (&Reflection::GetClass<UUID>() == &property->GetType())
		{
			UUID* uuid = static_cast<UUID*>(data);
			std::string str{};
			m_Reader.Read(str);
			if (HasToPopValue(property))
			{
				m_Reader.PopValue();
				m_HasToPopValue.erase(property);
			}
			*uuid = UUID::FromString(str);
			return false;
		}

		if (m_Reader.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			// Treat Plain Old Data as directly copyable
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				m_Reader.Read(data, rClass.Size);
				m_Reader.PopValue();
				return false;
			}
		}

		m_Reader.PushMap();
		return true;
	}

	bool SceneDeserializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_HasToPopValue.emplace(property);
			size_t size = m_Reader.PushSequence();

			for (size_t i = 0; i < size; ++i)
			{
				// this is a special case
				if (&collection.GetValueType() == &Reflection::GetClass<Actor>())
				{
					Actor& actor = m_Scene.CreateActor(); // this will add an element to the array
					actor.AddComponent<NameComponent>();
				}
				// this is also a special case, as we need to see what components we need to add before visiting them
				else if (property == m_ComponentsProperty) // we are assuming data is a pointer to the actor, this is how it is designed
				{
					m_Reader.PushIndex(i);
					m_Reader.PushMap();
					m_Reader.PushKey(0);

					std::string componentName{};
					m_Reader.Read(componentName);
					auto components = ComponentRegistry::GetComponents();
					auto it = std::find_if(components.begin(), components.end(), [&](const Reflection::Class* cComponent)
						{
							return !strcmp(cComponent->Name, componentName.c_str());
						});

					if (it != components.end())
					{
						Actor* actor = static_cast<Actor*>(data);
						if (!ComponentRegistry::InvokeHasComponent(**it, *actor))
						{
							ComponentRegistry::InvokeAddComponent(**it, *actor);
						}
					}
					else
					{
						SPX_CORE_ASSERT(false, "Component not found in registry!");
					}

					m_Reader.PopKey();
					m_Reader.PopMap();
					m_Reader.PopIndex();
				}
				else
				{
					collection.Add(data);
				}
			}
		}

		return true;
	}

	bool SceneDeserializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		size_t index = 0;
		if (m_Reader.FindKey(property->Name, index))
		{
			m_Reader.PushValue(index);
			m_HasToPopValue.emplace(property);
			size_t size = m_Reader.PushMap();

			for (size_t i = 0; i < size; ++i)
			{
				collection.Add(data, nullptr, nullptr);
			}
		}

		return true;
	}

	void SceneDeserializer::OnBeforeVisitEnum(const Reflection::Property* property, void* data)
	{
	}

	void SceneDeserializer::OnAfterVisitEnum(const Reflection::Property* property, void* data)
	{
	}

	void SceneDeserializer::OnBeforeVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->Name == property->GetType().Name)
		{
			m_Reader.PushMap();
		}
	}

	void SceneDeserializer::OnAfterVisitClass(const Reflection::Property* property, void* data)
	{
		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(property->GetType());

		/*
		using CustomSerializer = ::Sphynx::Serialization::CustomSerializer<TWriter>;
		if (const CustomSerializer* serializer = rClass.GetAttribute<CustomSerializer>())
		{
			return;
		}
		*/

		size_t value = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), value);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);

		if (&Reflection::GetClass<UUID>() == &property->GetType())
		{
			if (isIndex)
			{
				m_Reader.PopIndex();
			}
			return;
		}

		if (m_Reader.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				return;
			}
		}

		m_Reader.PopMap();


		if (!isIndex)
		{
			if (HasToPopValue(property))
			{
				m_Reader.PopValue();
				m_HasToPopValue.erase(property);
			}
		}
		else
		{
			m_Reader.PopIndex();
		}

		if (property->Name == property->GetType().Name)
		{
			m_Reader.PopMap();
		}
	}

	void SceneDeserializer::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
	}

	void SceneDeserializer::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (HasToPopValue(property))
		{
			m_Reader.PopSequence();
			m_Reader.PopValue();
			m_HasToPopValue.erase(property);
		}
	}

	void SceneDeserializer::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{

	}

	void SceneDeserializer::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		m_Reader.PopMap();

		if (HasToPopValue(property))
		{
			m_Reader.PopValue();
			m_HasToPopValue.erase(property);
		}
	}

	bool SceneDeserializer::HasToPopValue(const Reflection::Property* property)
	{
		return m_HasToPopValue.find(property) != m_HasToPopValue.end();
	}
}
