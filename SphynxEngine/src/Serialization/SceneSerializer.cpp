#include "spxpch.h"
#include "SceneSerializer.h"
#include "SerializationConstants.h"
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
	SceneSerializer::SceneSerializer(const Scene& scene, Writer& writer) :
		m_Scene(scene),
		m_Writer(writer)
	{}

	void SceneSerializer::Serialize()
	{
		// scene uuid and name
		m_Writer.Write(m_Scene.m_UUID);
		m_Writer.Write(m_Scene.m_Name);

		// num actors in scene
		m_Writer.Write(m_Scene.m_Actors.size());

		for (const Actor& actor : m_Scene.m_Actors)
		{
			// num of components in actor
			m_Writer.Write(actor.GetNumComponents());

			SerializeComponent<UUIDComponent>(actor);
			SerializeComponent<NameComponent>(actor);
			SerializeComponent<TransformComponent>(actor);
			SerializeComponent<LineRendererComponent>(actor);
			SerializeComponent<SpriteRendererComponent>(actor);
			SerializeComponent<BoxRendererComponent>(actor);
			SerializeComponent<Rigidbody2DComponent>(actor);
			SerializeComponent<BoxCollider2DComponent>(actor);
			SerializeComponent<CircleCollider2DComponent>(actor);
			SerializeComponent<CapsuleCollider2DComponent>(actor);
		}
	}
}