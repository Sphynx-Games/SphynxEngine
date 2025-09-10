#pragma once

#include <Core/Core.h>
#include <Scene/Scene.h>

#include "Scene/PrefabActor.h"


namespace Sphynx
{
	class EditorScene : public Scene
	{
		SPX_REFLECT_GENERATED_BODY()
	public:
		EditorScene();
		EditorScene(std::string name);

		PrefabActor& CreatePrefabActor(class Prefab* prefab);

		inline const Array<PrefabActor>& GetPrefabActors() const { return m_PrefabActors; }

	private:
		Array<PrefabActor> m_PrefabActors;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(Sphynx::EditorScene, /*SPHYNX_API*/)
SPX_REFLECT_INHERITANCE(Sphynx::Scene)
SPX_REFLECT_PROPERTY(m_UUID)
SPX_REFLECT_PROPERTY(m_Name)
SPX_REFLECT_PROPERTY(m_Actors)
SPX_REFLECT_CLASS_END(Sphynx::EditorScene, /*SPHYNX_API*/)