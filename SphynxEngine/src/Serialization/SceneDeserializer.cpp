#include "spxpch.h"
#include "SceneDeserializer.h"
#include "Reader.h"
#include "Scene/Scene.h"
#include "Component/Components.h"
#include "Renderer/Sprite.h"
#include "Renderer/Texture.h"
#include "Logging/Log.h"


namespace Sphynx
{
	SceneDeserializer::SceneDeserializer(Scene& scene, Reader&& reader) :
		m_Scene(scene),
		m_Reader(reader)
	{}

	void SceneDeserializer::Deserialize()
	{
		size_t size = m_Reader.PushMap();
		SPX_CORE_ASSERT(size == 1);
		m_Reader.PushKey(0);
		m_Reader.PopKey();

		m_Reader.PushValue(0);

		size = m_Reader.PushMap();
		SPX_CORE_ASSERT(size == 3);
		// scene uuid and name
		std::string key{};
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

				DeserializeComponent<UUIDComponent>(actor, componentName);
				DeserializeComponent<NameComponent>(actor, componentName);
				DeserializeComponent<TransformComponent>(actor, componentName);
				DeserializeComponent<CameraComponent>(actor, componentName);
				DeserializeComponent<LineRendererComponent>(actor, componentName);
				DeserializeComponent<SpriteRendererComponent>(actor, componentName);
				DeserializeComponent<BoxRendererComponent>(actor, componentName);
				DeserializeComponent<Rigidbody2DComponent>(actor, componentName);
				DeserializeComponent<BoxCollider2DComponent>(actor, componentName);
				DeserializeComponent<CircleCollider2DComponent>(actor, componentName);
				DeserializeComponent<CapsuleCollider2DComponent>(actor, componentName);
				
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
}