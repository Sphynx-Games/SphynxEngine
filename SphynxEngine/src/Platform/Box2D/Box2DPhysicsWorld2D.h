#pragma once

#include "Physics/PhysicsWorld2D.h"
#include "Box2DContactListener.h"

#include <box2d/b2_world.h>


namespace Sphynx
{
	class Box2DPhysicsWorld2D : public PhysicsWorld2D
	{
	public:
		Box2DPhysicsWorld2D();
		virtual ~Box2DPhysicsWorld2D();

		virtual void AddRigidbody(class Rigidbody2D* rigidbody) override;
		virtual void RemoveRigidbody(Rigidbody2D* rigidbody) override;

		virtual void Step(float timeStep) override;

	private:
		b2World m_PhysicsWorld;
		Box2DContactListener m_contactListener;
	};
}