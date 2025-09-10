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

		inline Prefab* GetPrefab() const { return m_Prefab; }

	private:
		Prefab* m_Prefab;

		friend class EditorScene;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS(Sphynx::PrefabActor, )