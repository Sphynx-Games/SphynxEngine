#include "spxpch.h"
#include "SceneSerializer.h"
#include "Writer.h"
#include "Scene/Scene.h"
#include "Component/Components.h"
#include "Renderer/Sprite.h"
#include "Renderer/Texture.h"
#include "Logging/Log.h"


namespace Sphynx
{
	SceneSerializer::SceneSerializer(const Scene& scene, Writer&& writer) :
		m_Scene(scene),
		m_Writer(writer)
	{}

	void SceneSerializer::Serialize()
	{
		m_Writer.PushMap();
		m_Writer.PushKey();
		m_Writer.Write("Scene");

		m_Writer.PushValue();
		// scene uuid and name
		m_Writer.PushMap();
		m_Writer.Write("m_UUID", m_Scene.m_UUID);
		m_Writer.Write("m_Name", m_Scene.m_Name);

		m_Writer.PushKey();
		m_Writer.Write("m_Actors");
		m_Writer.PushValue();
		m_Writer.PushSequence();
		for (const Actor& actor : m_Scene.m_Actors)
		{
			m_Writer.PushMap();
			m_Writer.Write("UUID", actor.GetComponent<UUIDComponent>().UUID);
			m_Writer.Write("Name", actor.GetComponent<NameComponent>().Name);

			m_Writer.PushKey(); m_Writer.Write("Components");
			m_Writer.PushValue();
			m_Writer.PushSequence();

			for (const Reflection::Class* componentClass : ComponentRegistry::GetComponents())
			{
				// do not copy UUIDComponent
				if (componentClass == &Reflection::GetClass<UUIDComponent>() || componentClass == &Reflection::GetClass<NameComponent>()) continue;

				SerializeComponent(*componentClass, actor);
			}
			m_Writer.PopSequence();
			m_Writer.PopMap();

		}
		m_Writer.PopSequence();

		m_Writer.PopMap();
		m_Writer.PopMap();
	}

	void SceneSerializer::SerializeComponent(const Reflection::Class& componentClass, const Actor& actor)
	{
		if (ComponentRegistry::InvokeHasComponent(componentClass, actor))
		{
			m_Writer.PushMap();
			m_Writer.PushKey();
			m_Writer.Write(componentClass.Name);

			m_Writer.PushKey();
			const void* component = ComponentRegistry::InvokeGetComponent(componentClass, actor);
			ReflectionSerializer serializer(component, componentClass, m_Writer);
			serializer.Serialize();
			m_Writer.PopMap();
		}
	}
}