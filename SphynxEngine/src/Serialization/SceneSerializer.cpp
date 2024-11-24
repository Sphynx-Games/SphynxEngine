#include "spxpch.h"
#include "SceneSerializer.h"
#include "Writer.h"
#include "Scene/Scene.h"
#include "Component/UUIDComponent.h"
#include "Component/NameComponent.h"
#include "Component/TransformComponent.h"
#include "Component/LineRendererComponent.h"
#include "Component/BoxRendererComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/Physics/Rigidbody2DComponent.h"
#include "Component/Physics/BoxCollider2DComponent.h"
#include "Component/Physics/CircleCollider2DComponent.h"
#include "Component/Physics/CapsuleCollider2DComponent.h"
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
			SerializeComponent<TransformComponent>(actor);
			SerializeComponent<LineRendererComponent>(actor);
			SerializeComponent<SpriteRendererComponent>(actor);
			SerializeComponent<BoxRendererComponent>(actor);
			SerializeComponent<Rigidbody2DComponent>(actor);
			SerializeComponent<BoxCollider2DComponent>(actor);
			SerializeComponent<CircleCollider2DComponent>(actor);
			SerializeComponent<CapsuleCollider2DComponent>(actor);
			m_Writer.PopSequence();
			m_Writer.PopMap();

		}
		m_Writer.PopSequence();

		m_Writer.PopMap();
		m_Writer.PopMap();
	}
}