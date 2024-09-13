#include "spxpch.h"
#include "PhysicsWorld2D.h"
#include "Rigidbody2D.h"
#include "Collider2D.h"
#include "Physics2D.h"

#ifdef SPX_PHYSICS_2D_BOX2D

#include "Platform/Box2D/Box2DData.h"

#else

#error No Physics 2D Engine selected

#endif


namespace Sphynx
{
#ifdef SPX_PHYSICS_2D_BOX2D

	PhysicsWorld2D::PhysicsWorld2D(Vector2f gravity) :
		m_Data(new PhysicsWorld2DData(gravity))
	{
	}

	PhysicsWorld2D::~PhysicsWorld2D()
	{
		delete m_Data;
	}

	void PhysicsWorld2D::Step(float timeStep, uint32_t velocityIterations, uint32_t positionIterations)
	{
		m_Data->PhysicsWorld.Step(timeStep, velocityIterations, positionIterations);
		OnPostStepPhysics.Execute();
	}
#else

#error No Physics 2D Engine selected

#endif
}