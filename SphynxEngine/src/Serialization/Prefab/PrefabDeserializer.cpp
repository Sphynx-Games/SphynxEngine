#include "spxpch.h"
#include "PrefabDeserializer.h"
#include "Serialization/Reader.h"
#include "Asset/Prefab/PrefabAsset.h"
#include "Component/Components.h"
#include "Logging/Log.h"


namespace Sphynx
{
	PrefabDeserializer::PrefabDeserializer(Prefab& prefab, Reader&& reader) :
		m_Prefab(prefab),
		m_Reader(reader)
	{}

	void PrefabDeserializer::Deserialize()
	{
		std::string key{};

		size_t size = m_Reader.PushMap();
		SPX_CORE_ASSERT(size == 1);
		m_Reader.PushKey(0);
		m_Reader.Read(key);
		SPX_CORE_ASSERT(key == "Prefab");
		m_Reader.PopKey();

		m_Reader.PushValue(0);
		Array<const Reflection::Class*> components = ComponentRegistry::GetComponents();

		size = m_Reader.PushMap();
		SPX_CORE_ASSERT(size == 3);
		m_Reader.Read(0, key, m_Prefab.GetComponent<UUIDComponent>().UUID);
		SPX_CORE_ASSERT(key == "UUID");
		m_Reader.Read(1, key, m_Prefab.AddComponent<NameComponent>().Name);
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
					DeserializeComponent(*componentClass);
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

		m_Reader.PopValue();
		m_Reader.PopMap();
	}

	void PrefabDeserializer::DeserializeComponent(const Reflection::Class& componentClass)
	{
		void* component = ComponentRegistry::InvokeAddComponent(componentClass, m_Prefab);
		ReflectionDeserializer deserializer(component, componentClass, m_Reader);
		deserializer.Deserialize();
	}
}