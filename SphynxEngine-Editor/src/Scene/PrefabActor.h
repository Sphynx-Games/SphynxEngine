#pragma once

#include <Scene/Actor.h>
#include <Asset/Prefab/PrefabAsset.h>


namespace Sphynx
{
	class PrefabActor : public Actor
	{
	public:
		PrefabActor() :
			Actor(),
			m_Prefab(nullptr)
		{}

		PrefabActor(uint32_t entityId, Scene* scene) :
			Actor(entityId, scene),
			m_Prefab(nullptr)
		{}

	private:
		Prefab* m_Prefab;

		friend class EditorScene;
	};
}