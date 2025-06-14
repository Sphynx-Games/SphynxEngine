#include "spxpch.h"
#include "SceneDeserializer.h"
#include "Serialization/Reader.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Component/Components.h"
#include "Logging/Log.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"


namespace Sphynx
{
	SceneDeserializer::SceneDeserializer(Scene& scene, Reader&& reader) :
		m_Scene(scene),
		m_Reader(reader)
	{}

	void SceneDeserializer::Deserialize()
	{
		std::string key{};

		size_t size = m_Reader.PushMap();
		SPX_CORE_ASSERT(size == 1);
		m_Reader.PushKey(0);
		m_Reader.Read(key);
		SPX_CORE_ASSERT(key == "Scene");
		m_Reader.PopKey();

		m_Reader.PushValue(0);

		size = m_Reader.PushMap();
		SPX_CORE_ASSERT(size == 3);
		// scene uuid and name
		m_Reader.Read(0, key, m_Scene.m_UUID);
		SPX_CORE_ASSERT(key == "m_UUID");
		m_Reader.Read(1, key, m_Scene.m_Name);
		SPX_CORE_ASSERT(key == "m_Name");

		m_Reader.PushKey(2);
		m_Reader.Read(key);
		SPX_CORE_ASSERT(key == "m_Actors");
		m_Reader.PopKey();

		m_Reader.PushValue(2);
		size_t actorsCount = m_Reader.PushSequence();
		Array<const Reflection::Class*> components = ComponentRegistry::GetComponents();

		for (size_t i = 0; i < actorsCount; ++i)
		{
			Actor& actor = m_Scene.CreateActor();
			m_Reader.PushIndex(i);
			size_t size = m_Reader.PushMap();
			SPX_CORE_ASSERT(size == 3);

			// Every actor has a UUIDComponent
			std::string key;
			m_Reader.Read(0, key, actor.GetComponent<UUIDComponent>().UUID);
			SPX_CORE_ASSERT(key == "UUID");
			m_Reader.Read(1, key, actor.AddComponent<NameComponent>().Name);
			SPX_CORE_ASSERT(key == "Name");

			m_Reader.PushKey(2);
			m_Reader.Read(key);
			SPX_CORE_ASSERT(key == "Components");
			m_Reader.PopKey();

			m_Reader.PushValue(2);
			size_t componentsCount = m_Reader.PushSequence();
			for (size_t j = 0; j < componentsCount; ++j)
			{
				m_Reader.PushIndex(j);
				size_t size = m_Reader.PushMap();
				SPX_CORE_ASSERT(size == 1);

				m_Reader.PushKey(0);
				std::string componentName; 
				m_Reader.Read(componentName);
				m_Reader.PopKey();

				m_Reader.PushValue(0);

				bool componentFound = false;
				for (const Reflection::Class* componentClass : components)
				{
					if (!strcmp(componentName.c_str(), componentClass->Name))
					{
						DeserializeComponent(*componentClass, actor);
						componentFound = true;
						break;
					}
				}

				if (!componentFound)
				{
					SPX_CORE_LOG_WARNING("Trying to deserialize component '{}' but no matching reflection class was found", componentName.c_str());
				}
				
				m_Reader.PopValue();
				m_Reader.PopMap();
				m_Reader.PopIndex();
			}
			m_Reader.PopSequence();
			m_Reader.PopValue();
			m_Reader.PopMap();
			m_Reader.PopIndex();
		}
		m_Reader.PopSequence();
		m_Reader.PopValue();

		m_Reader.PopMap();
		m_Reader.PopValue();
		m_Reader.PopMap();
	}

	void SceneDeserializer::DeserializeComponent(const Reflection::Class& componentClass, Actor& actor)
	{
		void* component = ComponentRegistry::InvokeAddComponent(componentClass, actor);
		ReflectionDeserializer deserializer(component, componentClass, m_Reader);
		deserializer.Deserialize();
	}
}