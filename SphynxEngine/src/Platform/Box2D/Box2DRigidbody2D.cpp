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
		Rigidbody2D(definition),
		m_Body(nullptr)
	{}

	Box2DRigidbody2D::~Box2DRigidbody2D()
	{
		m_Body = nullptr;
	};

	void Box2DRigidbody2D::AddCollider(Collider2D* collider)
	{
		Box2DCollider2D* colliderBox2D = dynamic_cast<Box2DCollider2D*>(collider);
		colliderBox2D->CreateFixtures(m_Body, m_Definition.Transform);
	}

	void Box2DRigidbody2D::RemoveCollider(Collider2D* collider)
	{
		Box2DCollider2D* colliderBox2D = dynamic_cast<Box2DCollider2D*>(collider);
		colliderBox2D->DestroyFixtures(m_Body);
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