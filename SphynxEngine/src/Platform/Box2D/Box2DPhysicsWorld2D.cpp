#include "Box2DPhysicsWorld2D.h"
#include "Box2DRigidbody2D.h"
#include "Box2DCollider2D.h"
#include "Physics/Collider2D.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <glm/glm.hpp>

namespace Sphynx
{
	Box2DPhysicsWorld2D::Box2DPhysicsWorld2D(Vector2f gravity) :
		m_PhysicsWorld(b2World({ gravity.X, gravity.Y })),
		m_contactListener()
	{
		m_PhysicsWorld.SetContactListener(&m_contactListener);
	}

	void Box2DPhysicsWorld2D::AddRigidbody(Rigidbody2D* rigidbody)
	{
		if (rigidbody == nullptr) return;

		Box2DRigidbody2D* rigidbodyBox2D = static_cast<Box2DRigidbody2D*>(rigidbody);

		b2BodyDef bodyDef;
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(rigidbodyBox2D);
		bodyDef.enabled = rigidbody->IsEnabled();
		bodyDef.type = Box2DRigidbody2D::RigidbodyType_To_Box2D(rigidbody->GetType());
		bodyDef.linearVelocity = { rigidbody->GetLinearVelocity().X, rigidbody->GetLinearVelocity().Y };
		bodyDef.angularVelocity = rigidbody->GetAngularVelocity();
		bodyDef.linearDamping = rigidbody->GetLinearDamping();
		bodyDef.angularDamping = rigidbody->GetAngularDamping();
		bodyDef.gravityScale = rigidbody->GetGravityScale();
		bodyDef.position = { rigidbody->GetPosition().X, rigidbody->GetPosition().Y };
		bodyDef.angle = glm::radians(rigidbody->GetRotation());

		b2Body* body = m_PhysicsWorld.CreateBody(&bodyDef);
		rigidbodyBox2D->m_Body = body;
		//rigidbodyBox2D->m_PhysicsWorld = this;
	}

	void Box2DPhysicsWorld2D::RemoveRigidbody(Rigidbody2D* rigidbody)
	{
		//if (rigidbody == nullptr) return;

		Box2DRigidbody2D* rigidbodyBox2D = static_cast<Box2DRigidbody2D*>(rigidbody);
		//if (rigidbodyBox2D->GetPhysicsWorld() != nullptr && rigidbodyBox2D->GetPhysicsWorld() == this)
		//{
			if (rigidbodyBox2D->m_Body != nullptr)
			{
				m_PhysicsWorld.DestroyBody(rigidbodyBox2D->m_Body);
				rigidbodyBox2D->m_Body = nullptr;
			}
			//rigidbodyBox2D->m_PhysicsWorld = nullptr;
		//}
	}

	void Box2DPhysicsWorld2D::AddCollider(Collider2D* collider, const RigidbodyDef& definition)
	{
		b2BodyDef bodyDef;
		bodyDef.position = { definition.Transform.Position.X, definition.Transform.Position.Y };
		bodyDef.angle = glm::radians(definition.Transform.Rotation.Z);

		b2Body* body = m_PhysicsWorld.CreateBody(&bodyDef);

		Box2DCollider2D* colliderBox2D = dynamic_cast<Box2DCollider2D*>(collider);
		colliderBox2D->CreateFixtures(body, definition.Transform);
	}

	void Box2DPhysicsWorld2D::Step(float timeStep, uint32_t velocityIterations, uint32_t positionIterations)
	{
		m_PhysicsWorld.Step(timeStep, velocityIterations, positionIterations);
		OnPostStepPhysics.Execute();
	}
}