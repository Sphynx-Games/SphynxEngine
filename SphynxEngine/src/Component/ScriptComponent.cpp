#include "ScriptComponent.h"
#include "Scripting/ScriptingManager.h"


namespace Sphynx
{
	ScriptComponent::ScriptComponent() :
		m_HasBegunPlay(false),
		m_Actor(nullptr)
	{
	}

	void ScriptComponent::BeginPlay()
	{
		// TODO: do something about this
		//if (m_HasBegunPlay) return;
		//m_HasBegunPlay = true;
	}

	void ScriptComponent::EndPlay()
	{
		// TODO: do something about this
		//if (!m_HasBegunPlay) return;
		//m_HasBegunPlay = false;
	}

	void ScriptComponent::Update(float deltaTime)
	{
		SPX_UNUSED(deltaTime);
	}
}