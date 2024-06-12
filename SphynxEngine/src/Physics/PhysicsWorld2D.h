#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Physics/Rigidbody2D.h"
#include "Physics/Physics2D.h"


namespace Sphynx
{
	class SPHYNX_API PhysicsWorld2D
	{
	protected:
		virtual ~PhysicsWorld2D() = default;

		virtual void AddRigidbody(class Rigidbody2D* rigidbody) = 0;
		virtual void RemoveRigidbody(Rigidbody2D* rigidbody) = 0;

		virtual void Step(float timeStep, uint32_t velocityIterations, uint32_t positionIterations) = 0;

	public:
		Delegate<void()> OnPostStepPhysics;

	public:
		friend class Physics2D;
	};
}
