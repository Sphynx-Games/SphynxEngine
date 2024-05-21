#include "Box2DPhysicsWorld2D.h"
#include "Box2DRigidbody2D.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <glm/glm.hpp>

namespace Sphynx
{
	Box2DPhysicsWorld2D::Box2DPhysicsWorld2D() :
		m_PhysicsWorld(b2World({ 0.0f, -10.0f })),
		m_contactListener()
	{
		m_PhysicsWorld.SetContactListener(&m_contactListener);
	}

	Box2DPhysicsWorld2D::~Box2DPhysicsWorld2D()
	{
		OnPostStepPhysics.Unbind();
	}

	void Box2DPhysicsWorld2D::AddRigidbody(Rigidbody2D* rigidbody)
	{
		if (rigidbody == nullptr) return;

		b2BodyDef bodyDef;
		RigidbodyDef rigidbodyDef = rigidbody->GetDefinition();
		bodyDef.enabled = rigidbodyDef.Enabled;
		bodyDef.type = Box2DRigidbody2D::RigidbodyType_To_Box2D(rigidbodyDef.Type);
		bodyDef.linearVelocity = { rigidbodyDef.LinearVelocity.X, rigidbodyDef.LinearVelocity.Y };
		bodyDef.angularVelocity = rigidbodyDef.AngularVelocity;
		bodyDef.linearDamping = rigidbodyDef.LinearDamping;
		bodyDef.angularDamping = rigidbodyDef.AngularDamping;
		bodyDef.gravityScale = rigidbodyDef.GravityScale;
		bodyDef.position = { rigidbodyDef.Transform.Position.X, rigidbodyDef.Transform.Position.Y };
		bodyDef.angle = glm::radians(rigidbodyDef.Transform.Rotation.Z);

		b2Body* body = m_PhysicsWorld.CreateBody(&bodyDef);
		Box2DRigidbody2D* rigidbodyBox2D = static_cast<Box2DRigidbody2D*>(rigidbody);
		rigidbodyBox2D->m_Body = body;
		rigidbodyBox2D->m_PhysicWorld = this;
	}

	void Box2DPhysicsWorld2D::RemoveRigidbody(Rigidbody2D* rigidbody)
	{
		if (rigidbody == nullptr) return;

		Box2DRigidbody2D* rigidbodyBox2D = static_cast<Box2DRigidbody2D*>(rigidbody);
		if (rigidbodyBox2D->GetPhysicWorld() != nullptr && rigidbodyBox2D->GetPhysicWorld() == this)
		{
			if (rigidbodyBox2D->m_Body != nullptr)
			{
				m_PhysicsWorld.DestroyBody(rigidbodyBox2D->m_Body);
				rigidbodyBox2D->m_Body = nullptr;
			}
			rigidbodyBox2D->m_PhysicWorld = nullptr;
		}
	}

	void Box2DPhysicsWorld2D::Step(float timeStep)
	{
		m_PhysicsWorld.Step(timeStep, 8, 3);
		OnPostStepPhysics.Execute();
	}
}