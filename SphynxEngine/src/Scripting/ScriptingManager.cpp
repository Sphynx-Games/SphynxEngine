#include "ScriptingManager.h"
#include "Scene/Scene.h"
#include "Component/ScriptComponent.h"
#include "Container/Array.h"


namespace Sphynx
{
	void ScriptingManager::Update(const Scene& scene, float deltaTime)
	{
		for (void* scriptPtr : ComponentRegistry::InvokeGetSceneComponents(Reflection::GetClass<ScriptComponent>(), scene))
		{
			ScriptComponent* script = static_cast<ScriptComponent*>(scriptPtr);
			script->Update(deltaTime);
		}
	}

}
