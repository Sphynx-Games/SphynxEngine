#include "spxpch.h"
#include "SceneDeserializer.h"
#include "SerializationConstants.h"
#include "Reader.h"
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
	SceneDeserializer::SceneDeserializer(Scene& scene, Reader& reader) :
		m_Scene(scene),
		m_Reader(reader)
	{}

	void SceneDeserializer::Deserialize()
	{
		// scene uuid and name
		m_Reader.Read(m_Scene.m_UUID);
		m_Reader.Read(m_Scene.m_Name);

		// num actors in scene
		size_t numActors;
		m_Reader.Read(numActors);

		for (size_t i = 0; i < numActors; ++i)
		{
			Actor& actor = m_Scene.CreateActor();

			// num of components in actor
			size_t numComponents;
			m_Reader.Read(numComponents);

			// UUID Component : every actor has a UUIDComponent
			std::string uuidComponentName;
			m_Reader.Read(uuidComponentName);
			UUIDComponent& component = actor.GetComponent<UUIDComponent>();
			ReflectionDeserializer deserializer(component, m_Reader);
			deserializer.Deserialize();

			for (size_t j = 0; j < numComponents - 1; ++j)
			{
				std::string componentName;
				m_Reader.Read(componentName);

				DeserializeComponent<UUIDComponent>(actor, componentName);
				DeserializeComponent<NameComponent>(actor, componentName);
				DeserializeComponent<TransformComponent>(actor, componentName);
				DeserializeComponent<LineRendererComponent>(actor, componentName);
				DeserializeComponent<SpriteRendererComponent>(actor, componentName);
				DeserializeComponent<BoxRendererComponent>(actor, componentName);
				DeserializeComponent<Rigidbody2DComponent>(actor, componentName);
				DeserializeComponent<BoxCollider2DComponent>(actor, componentName);
				DeserializeComponent<CircleCollider2DComponent>(actor, componentName);
				DeserializeComponent<CapsuleCollider2DComponent>(actor, componentName);
			}
		}
	}
}