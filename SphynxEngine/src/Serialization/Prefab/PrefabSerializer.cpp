#include "spxpch.h"
#include "PrefabSerializer.h"
#include "Serialization/Writer.h"
#include "Asset/Prefab/PrefabAsset.h"
#include "Component/Components.h"
#include "Logging/Log.h"


namespace Sphynx
{
	PrefabSerializer::PrefabSerializer(const Prefab& prefab, Writer&& writer) :
		m_Prefab(prefab),
		m_Writer(writer)
	{}

	void PrefabSerializer::Serialize()
	{
		m_Writer.PushMap();
		m_Writer.PushKey(); m_Writer.Write("Prefab");
		m_Writer.PushValue();
		m_Writer.PushMap();
		m_Writer.Write("UUID", m_Prefab.GetComponent<UUIDComponent>().UUID);
		m_Writer.Write("Name", m_Prefab.GetComponent<NameComponent>().Name);
		m_Writer.PushKey(); m_Writer.Write("Components");
		m_Writer.PushValue();
		m_Writer.PushSequence();
		for (const Reflection::Class* componentClass : ComponentRegistry::GetComponents())
		{
			// do not copy UUIDComponent and NameComponent
			if (componentClass == &Reflection::GetClass<UUIDComponent>() || componentClass == &Reflection::GetClass<NameComponent>()) continue;

			SerializeComponent(*componentClass);
		}
		m_Writer.PopSequence();
		m_Writer.PopMap();
		m_Writer.PopMap();
	}

	void PrefabSerializer::SerializeComponent(const Reflection::Class& componentClass)
	{
		if (ComponentRegistry::InvokeHasComponent(componentClass, m_Prefab))
		{
			m_Writer.PushMap();
			m_Writer.PushKey(); m_Writer.Write(componentClass.Name);

			m_Writer.PushKey();
			const void* component = ComponentRegistry::InvokeGetComponent(componentClass, m_Prefab);
			ReflectionSerializer serializer(component, componentClass, m_Writer);
			serializer.Serialize();
			m_Writer.PopMap();
		}
	}
}