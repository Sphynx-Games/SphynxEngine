#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Physics/Rigidbody2D.h"


namespace Sphynx
{
	class SPHYNX_API PhysicsWorld2D
	{
	public:
		virtual ~PhysicsWorld2D() = default;

		virtual void AddRigidbody(class Rigidbody2D* rigidbody) = 0;
		virtual void RemoveRigidbody(Rigidbody2D* rigidbody) = 0;

		virtual void Step(float timeStep) = 0;

	public:
		Delegate<void()> OnPostStepPhysics;
	};
}
