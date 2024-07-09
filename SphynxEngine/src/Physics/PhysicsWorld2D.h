#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"
#include "Core/Delegate.h"


namespace Sphynx
{
	class SPHYNX_API PhysicsWorld2D
	{
	public:
		PhysicsWorld2D(Vector2f gravity);
		inline struct PhysicsWorld2DData* GetData() const { return m_Data; }

	protected:		
		~PhysicsWorld2D();

		void Step(float timeStep, uint32_t velocityIterations, uint32_t positionIterations);

	public:
		Delegate<void()> OnPostStepPhysics;

	protected:
		PhysicsWorld2DData* m_Data;

		friend class Physics2D;
		friend class Rigidbody2D;
		friend class Collider2D;
	};
}
