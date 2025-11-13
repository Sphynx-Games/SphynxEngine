#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Scene/Actor.h"


namespace Sphynx
{
	// parent component of all the components created by a user of the engine
	class SPHYNX_API ScriptComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(ScriptComponent);

	public:
		ScriptComponent();

		inline bool HasBegunPlay() const { return m_HasBegunPlay; }
		inline Actor* GetActor() const { return m_Actor; }

	protected:
		virtual void BeginPlay();
		virtual void EndPlay();
		virtual void Update(float deltaTime);

	private:
		bool m_HasBegunPlay;
		Actor* m_Actor;

		friend class Actor;
		friend class ScriptingManager;
	};
}

#include "Reflection/Reflection.h"


SPX_REFLECT_CLASS_BEGIN(Sphynx::ScriptComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_CLASS_END(Sphynx::ScriptComponent, SPHYNX_API)