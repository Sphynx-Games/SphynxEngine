#include "ScriptComponent.h"


namespace Sphynx
{
	ScriptComponent::ScriptComponent() : m_HasBegunPlay(false)
	{
	}

	void ScriptComponent::BeginPlay()
	{
		if (m_HasBegunPlay) return;
		m_HasBegunPlay = true;
	}

	void ScriptComponent::EndPlay()
	{
		if (!m_HasBegunPlay) return;
		m_HasBegunPlay = false;
	}

	void ScriptComponent::Update(float deltaTime)
	{
	}
}