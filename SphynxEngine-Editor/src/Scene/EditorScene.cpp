#include "EditorScene.h"

#include <Asset/Prefab/PrefabAsset.h>


namespace Sphynx
{
	EditorScene::EditorScene() :
		Scene(),
		m_PrefabActors()
	{}

	EditorScene::EditorScene(std::string name) :
		Scene(name),
		m_PrefabActors()
	{}

	PrefabActor& EditorScene::CreatePrefabActor(Prefab* prefab)
	{
		const Actor& actor = CreateActor();
		PrefabActor& prefabActor = m_PrefabActors.Emplace(static_cast<uint32_t>(actor), this);
		prefabActor.m_Prefab = prefab;
		Actor::CloneComponents(*prefab, prefabActor);
		return prefabActor;
	}
}