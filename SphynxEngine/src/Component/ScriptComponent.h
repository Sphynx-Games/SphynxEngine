#pragma once

#include "Core/Core.h"
#include "Common.h"


namespace Sphynx
{
	// parent component of all the components created by a user of the engine
	class SPHYNX_API ScriptComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(ScriptComponent);

	public:
		ScriptComponent();

	public:
		virtual void BeginPlay();
		virtual void EndPlay();
		virtual void Update(float deltaTime);

		inline bool HasBegunPlay() const { return m_HasBegunPlay; }

	private:
		bool m_HasBegunPlay;
	};
}

#include "Reflection/Reflection.h"


SPX_REFLECT_CLASS_BEGIN(Sphynx::ScriptComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_CLASS_END(Sphynx::ScriptComponent)