#pragma once

#include <Core/Core.h>
#include <Scene/Scene.h>

#include "Scene/PrefabActor.h"


namespace Sphynx
{
	class EditorScene : public Scene
	{
	public:
		EditorScene();
		EditorScene(std::string name);

		PrefabActor& CreatePrefabActor(class Prefab* prefab);

	private:
		Array<PrefabActor> m_PrefabActors;
	};
}