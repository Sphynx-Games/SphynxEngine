#include "spxpch.h"
#include "EditorSceneDeserializer.h"
#include "EditorScene.h"
#include <Serialization/Reader.h>
#include <Component/Components.h>
#include <Asset/AssetManager.h>
#include <Asset/Prefab/PrefabAsset.h>
/*#include "Scene/Actor.h"
#include "Logging/Log.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"*/


namespace Sphynx
{
	EditorSceneDeserializer::EditorSceneDeserializer(EditorScene& scene, Reader&& reader) :
		SceneDeserializer(scene, std::move(reader))
	{}

	void EditorSceneDeserializer::Deserialize()
	{
		size_t actorsCount = DeserializeSceneProperties();
		for (size_t i = 0; i < actorsCount; ++i)
		{
			m_Reader.PushIndex(i);
			size_t size = m_Reader.PushMap();
			SPX_CORE_ASSERT(size == 3 || size == 4, "Actor is not well defined!! Impossible to deserialize.");
			if (size == 3)
			{
				DeserializeActor();
			}
			else
			{
				std::string key, value;
				AssetHandle assetHandle;
				m_Reader.Read(0, key, assetHandle);
				SPX_CORE_ASSERT(key == "Prefab");

				std::shared_ptr<Asset<Prefab>> prefab = AssetManager::GetAsset<Prefab>(assetHandle);
				DeserializePrefabActor(prefab->Asset);
			}
		}
		EndDeserialize();
	}

	void EditorSceneDeserializer::DeserializePrefabActor(Prefab* prefab)
	{
		PrefabActor& actor = static_cast<EditorScene&>(m_Scene).CreatePrefabActor(prefab);

		// Every actor has a UUID and NameComponent
		std::string key;
		m_Reader.Read(1, key, actor.GetComponent<UUIDComponent>().UUID);
		SPX_CORE_ASSERT(key == "UUID");
		m_Reader.Read(2, key, actor.AddComponent<NameComponent>().Name);
		SPX_CORE_ASSERT(key == "Name");

		m_Reader.PushKey(3);
		m_Reader.Read(key);
		SPX_CORE_ASSERT(key == "Components");
		m_Reader.PopKey();

		m_Reader.PushValue(3);
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
			for (const Reflection::Class* componentClass : ComponentRegistry::GetComponents())
			{
				if (!strcmp(componentName.c_str(), componentClass->Name))
				{
					DeserializePrefabComponent(*componentClass, actor);
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

	void EditorSceneDeserializer::DeserializePrefabComponent(const Reflection::Class& componentClass, Actor& actor)
	{
		if (ComponentRegistry::InvokeHasComponent(componentClass, actor, false))
		{
			if (!m_Reader.IsMap())
			{
				ComponentRegistry::InvokeRemoveComponent(componentClass, actor);
			}
		}
		else
		{
			ComponentRegistry::InvokeAddComponent(componentClass, actor);
		}
		void* component = ComponentRegistry::InvokeGetComponent(componentClass, actor, false);
		ReflectionDeserializer deserializer(component, componentClass, m_Reader);
		deserializer.Deserialize();
	}
}