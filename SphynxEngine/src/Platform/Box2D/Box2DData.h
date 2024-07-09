#pragma once

#include "Container/Map.h"
#include "Physics/PhysicsWorld2D.h"
#include "Physics/Rigidbody2D.h"
#include "Physics/Collider2D.h"
#include "Platform/Box2D/Box2DContactListener.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

namespace Sphynx
{
	struct PhysicsWorld2DData
	{
	public:
		PhysicsWorld2DData(Vector2f gravity) :
			PhysicsWorld(b2World({ gravity.X, gravity.Y })),
			ContactListener()
		{
			PhysicsWorld.SetContactListener(&ContactListener);
		};

	public:
		b2World PhysicsWorld;
		Box2DContactListener ContactListener;

		friend PhysicsWorld2D;
	};

	struct Rigidbody2DData
	{
	public:
		Rigidbody2DData() : Body(nullptr) {}
		~Rigidbody2DData() { Body = nullptr; }

	public:
		b2Body* Body;

		friend Rigidbody2D;
	};

	struct Collider2DData
	{
	public:
		Collider2DData() : Fixtures() {};
		~Collider2DData() { Fixtures.RemoveAll(); };

	public:
		Array<b2Fixture*> Fixtures;

		friend Collider2D;
	};
}