#include "spxpch.h"
#include "ActorSerializer.h"
#include "Serialization/Writer.h"
#include "Component/Components.h"
#include "Logging/Log.h"
#include "../Scene/SceneSerializer.h"
#include "Reflection/PropertyTree.h"


namespace Sphynx
{
	ActorSerializer::ActorSerializer(const Actor& actor, Writer&& writer) :
		ActorSerializer(actor, Reflection::GetClass<Actor>(), std::move(writer))
	{

	}

	ActorSerializer::ActorSerializer(const class Actor& actor, const Reflection::Class& cClass, class Writer&& writer) :
		m_Actor(actor),
		m_Type(cClass),
		m_Writer(writer)
	{

	}

	void ActorSerializer::Serialize()
	{
		m_Writer.PushMap();
		m_Writer.PushKey(); m_Writer.Write(m_Type.Name); // Generally Sphynx::Actor or Sphynx::Prefab
		m_Writer.PushValue();
		m_Writer.PushMap();
		m_Writer.Write("UUID", m_Actor.GetComponent<UUIDComponent>().UUID);
		m_Writer.Write("Name", m_Actor.GetComponent<NameComponent>().Name);
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

	void ActorSerializer::SerializeComponent(const Reflection::Class& componentClass)
	{
		if (ComponentRegistry::InvokeHasComponent(componentClass, m_Actor))
		{
			void* component = ComponentRegistry::InvokeGetComponent(componentClass, m_Actor);
			Scene scene{};
			Reflection::PropertyTree tree{ componentClass, component };
			SceneSerializer serializer(scene, Writer{ m_Writer });
			serializer.ConfigurePropertyTree(tree, m_Type);
			tree.Traverse(serializer);
		}
	}
}