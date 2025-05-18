#include "ScriptingManager.h"
#include "Scene/Scene.h"
#include "Component/ScriptComponent.h"
#include "Container/Array.h"


namespace Sphynx
{
	void ScriptingManager::Update(const Scene& scene, float deltaTime)
	{
		Array<ScriptComponent*> components = scene.GetComponents<ScriptComponent>();
		for (ScriptComponent* script : components)
		{
			script->Update(deltaTime);
		}
	}

}
