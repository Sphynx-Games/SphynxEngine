#include "Box2DRigidbody2D.h"
#include "Box2DPhysicsWorld2D.h"
#include "Physics/Collider2D.h"
#include "Box2DCollider2D.h"

#include <glm/glm.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

namespace Sphynx
{
	Box2DRigidbody2D::Box2DRigidbody2D(RigidbodyDef definition) :
		m_Body(nullptr)
	{
		m_Definition = definition;
		m_PhysicsWorld = nullptr;
	}

	Box2DRigidbody2D::~Box2DRigidbody2D()
	{
		m_Body = nullptr;
		Rigidbody2D::~Rigidbody2D();
	};

	void Box2DRigidbody2D::AddCollider(Collider2D* collider)
	{
		// BOX COLLIDERS
		if (Box2DBoxCollider2D* colliderBox2D = dynamic_cast<Box2DBoxCollider2D*>(collider))
		{
			b2PolygonShape shape;
			shape.SetAsBox(
				(colliderBox2D->GetSize().X * m_Definition.Transform.Scale.X) / 2.0f,
				(colliderBox2D->GetSize().Y * m_Definition.Transform.Scale.Y) / 2.0f,
				{ colliderBox2D->GetOffset().X * m_Definition.Transform.Scale.X, colliderBox2D->GetOffset().Y * m_Definition.Transform.Scale.Y },
				0.0f
			);

			colliderBox2D->m_Fixtures.push_back(m_Body->CreateFixture(&shape, 1.0f));
		}

		// CIRCLE COLLIDERS
		else if (Box2DCircleCollider2D* colliderBox2D = dynamic_cast<Box2DCircleCollider2D*>(collider))
		{
			b2CircleShape shape;
			shape.m_p = { colliderBox2D->GetOffset().X * m_Definition.Transform.Scale.X, colliderBox2D->GetOffset().Y * m_Definition.Transform.Scale.Y };
			shape.m_radius = colliderBox2D->GetRadius() * std::max(m_Definition.Transform.Scale.X, m_Definition.Transform.Scale.Y);

			colliderBox2D->m_Fixtures.push_back(m_Body->CreateFixture(&shape, 1.0f));
		}

		// CAPSULE COLLIDERS
		else if (Box2DCapsuleCollider2D* colliderBox2D = dynamic_cast<Box2DCapsuleCollider2D*>(collider))
		{
			Vector2f capsuleSize = colliderBox2D->GetSize() * Vector2f{ m_Definition.Transform.Scale.X, m_Definition.Transform.Scale.Y };
			Vector2f scaledOffset = {
				colliderBox2D->GetOffset().X * m_Definition.Transform.Scale.X,
				colliderBox2D->GetOffset().Y * m_Definition.Transform.Scale.Y
			};

			if (capsuleSize.X == capsuleSize.Y)
			{
				// 1 CIRCLE
				b2CircleShape circle;
				circle.m_p = b2Vec2{ scaledOffset.X, scaledOffset.Y };
				circle.m_radius = capsuleSize.X / 2.0f;

				colliderBox2D->m_Fixtures.push_back(m_Body->CreateFixture(&circle, 1.0f));
			}
			else
			{
				// 1 BOX + 2 CIRCLES
				Vector2f boxSize =
				{
					(capsuleSize.X > capsuleSize.Y) ? capsuleSize.X - capsuleSize.Y : capsuleSize.X,
					(capsuleSize.X > capsuleSize.Y) ? capsuleSize.Y : capsuleSize.Y - capsuleSize.X
				};

				b2PolygonShape box;
				box.SetAsBox(
					boxSize.X / 2.0f,
					boxSize.Y / 2.0f,
					{ scaledOffset.X, scaledOffset.Y },
					0.0f
				);

				b2CircleShape circleA; // right, up
				circleA.m_p = (capsuleSize.X > capsuleSize.Y)
					? b2Vec2{ (boxSize.X / 2.0f) + scaledOffset.X, scaledOffset.Y }
				    : b2Vec2{ scaledOffset.X, (boxSize.Y / 2.0f) + scaledOffset.Y };
				circleA.m_radius = (capsuleSize.X > capsuleSize.Y) ? boxSize.Y / 2.0f : boxSize.X / 2.0f;
				b2CircleShape circleB; // left, down
				circleB.m_p = (capsuleSize.X > capsuleSize.Y)
					? b2Vec2{ (-boxSize.X / 2.0f) + scaledOffset.X, scaledOffset.Y }
				    : b2Vec2{ scaledOffset.X, (-boxSize.Y / 2.0f) + scaledOffset.Y };
				circleB.m_radius = (capsuleSize.X > capsuleSize.Y) ? boxSize.Y / 2.0f : boxSize.X / 2.0f;

				colliderBox2D->m_Fixtures.push_back(m_Body->CreateFixture(&box, 1.0f));
				colliderBox2D->m_Fixtures.push_back(m_Body->CreateFixture(&circleA, 1.0f));
				colliderBox2D->m_Fixtures.push_back(m_Body->CreateFixture(&circleB, 1.0f));
			}
		}
		
		//collider->m_Rigidbody = this;
	}

	void Box2DRigidbody2D::RemoveCollider(Collider2D* collider)
	{
		std::vector<b2Fixture*>* fixtures = &std::vector<b2Fixture*>();
		if (Box2DBoxCollider2D* colliderBox2D = dynamic_cast<Box2DBoxCollider2D*>(collider))
		{
			fixtures = &colliderBox2D->m_Fixtures;
		}
		else if (Box2DCircleCollider2D* colliderBox2D = dynamic_cast<Box2DCircleCollider2D*>(collider))
		{
			fixtures = &colliderBox2D->m_Fixtures;
		}
		else if (Box2DCapsuleCollider2D* colliderBox2D = dynamic_cast<Box2DCapsuleCollider2D*>(collider))
		{
			fixtures = &colliderBox2D->m_Fixtures;
		}
			
		if (fixtures->size() > 0)
		{
			for (b2Fixture* fix : *fixtures)
			{
				m_Body->DestroyFixture(fix);
			}
			fixtures->clear();

			//collider->m_Rigidbody = nullptr;
			//collider->SetRigidbody(nullptr);
		}
	}

	Vector2f Box2DRigidbody2D::GetPosition() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::GetPosition();
		}
		return Vector2f(m_Body->GetPosition().x, m_Body->GetPosition().y);
	}

	float Box2DRigidbody2D::GetRotation() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::GetRotation();
		}
		return glm::degrees(m_Body->GetAngle());
	}

	bool Box2DRigidbody2D::IsEnabled() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::IsEnabled();
		}
		return m_Body->IsEnabled();
	}

	void Box2DRigidbody2D::SetEnabled(bool enable)
	{
		Rigidbody2D::SetEnabled(enable);
		if (m_Body != nullptr)
		{
			m_Body->SetEnabled(enable);
		}
	}

	RigidbodyType Box2DRigidbody2D::GetType() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::GetType();
		}
		return Box2D_To_RigidbodyType(m_Body->GetType());
	}

	void Box2DRigidbody2D::SetType(RigidbodyType type)
	{
		Rigidbody2D::SetType(type);
		if (m_Body != nullptr)
		{
			m_Body->SetType(RigidbodyType_To_Box2D(type));
		}
	}

	Vector2f Box2DRigidbody2D::GetLinearVelocity() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::GetLinearVelocity();
		}
		b2Vec2 velocity = m_Body->GetLinearVelocity();
		return Vector2f(velocity.x, velocity.y);
	}

	void Box2DRigidbody2D::SetLinearVelocity(Vector2f velocity)
	{
		Rigidbody2D::SetLinearVelocity(velocity);
		if (m_Body != nullptr)
		{
			m_Body->SetLinearVelocity({ velocity.X, velocity.Y });
		}
	}

	float Box2DRigidbody2D::GetAngularVelocity() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::GetAngularVelocity();
		}
		return m_Body->GetAngularVelocity();
	}

	void Box2DRigidbody2D::SetAngularVelocity(float velocity)
	{
		Rigidbody2D::SetAngularVelocity(velocity);
		if (m_Body != nullptr)
		{
			m_Body->SetAngularVelocity(velocity);
		}
	}

	float Box2DRigidbody2D::GetLinearDamping() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::GetLinearDamping();
		}
		return m_Body->GetLinearDamping();
	}

	void Box2DRigidbody2D::SetLinearDamping(float damping)
	{
		Rigidbody2D::SetLinearDamping(damping);
		if (m_Body != nullptr)
		{
			m_Body->SetLinearDamping(damping);
		}
	}

	float Box2DRigidbody2D::GetAngularDamping() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::GetAngularDamping();
		}
		return m_Body->GetAngularDamping();
	}

	void Box2DRigidbody2D::SetAngularDamping(float damping)
	{
		Rigidbody2D::SetAngularDamping(damping);
		if (m_Body != nullptr)
		{
			m_Body->SetAngularDamping(damping);
		}
	}

	float Box2DRigidbody2D::GetGravityScale() const
	{
		if (m_Body == nullptr)
		{
			return Rigidbody2D::GetGravityScale();
		}
		return m_Body->GetGravityScale();
	}

	void Box2DRigidbody2D::SetGravityScale(float gravityScale)
	{
		Rigidbody2D::SetGravityScale(gravityScale);
		if (m_Body != nullptr)
		{
			m_Body->SetGravityScale(gravityScale);
		}
	}

	b2BodyType Box2DRigidbody2D::RigidbodyType_To_Box2D(RigidbodyType type)
	{
		switch (type)
		{
		case RigidbodyType::STATIC:     return b2BodyType::b2_staticBody;
		case RigidbodyType::DYNAMIC:    return b2BodyType::b2_dynamicBody;
		case RigidbodyType::KINEMATIC:  return b2BodyType::b2_kinematicBody;
		}

		SPX_CORE_ASSERT(false);
		return b2BodyType::b2_staticBody;
	}

	RigidbodyType Box2DRigidbody2D::Box2D_To_RigidbodyType(b2BodyType type)
	{
		switch (type)
		{
		case b2BodyType::b2_staticBody:     return RigidbodyType::STATIC;
		case b2BodyType::b2_dynamicBody:    return RigidbodyType::DYNAMIC;
		case b2BodyType::b2_kinematicBody:  return RigidbodyType::KINEMATIC;
		}

		SPX_CORE_ASSERT(false);
		return RigidbodyType::STATIC;
	}
}