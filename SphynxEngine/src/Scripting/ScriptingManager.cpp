#include "ScriptingManager.h"
#include "Scene/Scene.h"
#include "Component/ScriptComponent.h"
#include "Container/Array.h"


namespace Sphynx
{
	void ScriptingManager::BeginPlay(const Scene& scene)
	{
		for (void* scriptPtr : ComponentRegistry::InvokeGetSceneComponents(Reflection::GetClass<ScriptComponent>(), scene))
		{
			ScriptComponent* script = static_cast<ScriptComponent*>(scriptPtr);
			if (!script->HasBegunPlay())
			{
				script->BeginPlay();
				script->m_HasBegunPlay = true;
			}
		}
	}

	void ScriptingManager::Update(const Scene& scene, float deltaTime)
	{
		for (void* scriptPtr : ComponentRegistry::InvokeGetSceneComponents(Reflection::GetClass<ScriptComponent>(), scene))
		{
			ScriptComponent* script = static_cast<ScriptComponent*>(scriptPtr);
			if (script->HasBegunPlay())
			{
				script->Update(deltaTime);
			}
		}
	}

	void ScriptingManager::EndPlay(const Scene& scene)
	{
		for (void* scriptPtr : ComponentRegistry::InvokeGetSceneComponents(Reflection::GetClass<ScriptComponent>(), scene))
		{
			ScriptComponent* script = static_cast<ScriptComponent*>(scriptPtr);
			if (script->HasBegunPlay())
			{
				script->EndPlay();
				script->m_HasBegunPlay = false;
			}
		}
	}

}
