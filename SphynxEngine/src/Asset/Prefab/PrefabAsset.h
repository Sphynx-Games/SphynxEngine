#pragma once

#include "Core/Core.h"
#include "Asset/Asset.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"


namespace Sphynx
{
	class SPHYNX_API Prefab : public Actor {
	public:
		Prefab()
		{
			m_EntityID = GetPrefabScene().CreateActor();
			m_Scene = &GetPrefabScene();
			m_numComponents = 0;
		}

		inline static Scene& GetPrefabScene()
		{
			static Scene s_PrefabScene{};
			return s_PrefabScene;
		}

	private:
		friend class PrefabSerializer;
		friend class PrefabDeserializer;
	};

	GENERATE_ASSETTYPE_CONVERSOR(Prefab);

}

#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS(Sphynx::Prefab, SPHYNX_API)