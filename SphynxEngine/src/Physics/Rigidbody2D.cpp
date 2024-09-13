#include "spxpch.h"
#include "Rigidbody2D.h"
#include "PhysicsWorld2D.h"
#include "Collider2D.h"
#include "Physics2D.h"

#ifdef SPX_PHYSICS_2D_BOX2D

#include "Platform/Box2D/Box2DData.h"

#include <glm/glm.hpp>

#else

#error No Physics 2D Engine selected

#endif


namespace Sphynx
{
#ifdef SPX_PHYSICS_2D_BOX2D

	b2BodyType RigidbodyType_To_Box2D(RigidbodyType type);
	RigidbodyType Box2D_To_RigidbodyType(b2BodyType type);


	Rigidbody2D::Rigidbody2D(RigidbodyDef definition) :
		m_Definition(definition),
		m_PhysicsWorld(nullptr),
		m_Data(new Rigidbody2DData())
	{
	}

	Rigidbody2D::~Rigidbody2D()
	{
		m_PhysicsWorld = nullptr;
		delete m_Data;
	};

	void Rigidbody2D::AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld)
	{
		b2BodyDef bodyDef;
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
		bodyDef.enabled = IsEnabled();
		bodyDef.type = RigidbodyType_To_Box2D(GetType());
		bodyDef.linearVelocity = { GetLinearVelocity().X, GetLinearVelocity().Y };
		bodyDef.angularVelocity = GetAngularVelocity();
		bodyDef.linearDamping = GetLinearDamping();
		bodyDef.angularDamping = GetAngularDamping();
		bodyDef.gravityScale = GetGravityScale();
		bodyDef.position = { GetPosition().X, GetPosition().Y };
		bodyDef.angle = glm::radians(GetRotation());

		b2Body* body = physicsWorld->GetData()->PhysicsWorld.CreateBody(&bodyDef);
		m_Data->Body = body;

		m_PhysicsWorld = physicsWorld;
	}

	void Rigidbody2D::Detach()
	{
		if (m_Data->Body != nullptr)
		{
			m_PhysicsWorld->GetData()->PhysicsWorld.DestroyBody(m_Data->Body);
			m_Data->Body = nullptr;
		}
		m_PhysicsWorld = nullptr;
	}

	Vector2f Rigidbody2D::GetPosition() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::GetPositionDef();
		}
		return Vector2f(m_Data->Body->GetPosition().x, m_Data->Body->GetPosition().y);
	}

	float Rigidbody2D::GetRotation() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::GetRotationDef();
		}
		return glm::degrees(m_Data->Body->GetAngle());
	}

	Transform Rigidbody2D::GetTransform() const
	{
		Vector2f pos = GetPosition();
		float rot = GetRotation();
		Transform trans = { { pos.X, pos.Y, 0.0f }, m_Definition.Transform.Scale, {0.0f, 0.0f, rot } };
		return trans;
	}

	bool Rigidbody2D::IsEnabled() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::IsEnabledDef();
		}
		return m_Data->Body->IsEnabled();
	}

	void Rigidbody2D::SetEnabled(bool enable)
	{
		Rigidbody2D::SetEnabledDef(enable);
		if (m_Data->Body != nullptr)
		{
			m_Data->Body->SetEnabled(enable);
		}
	}

	RigidbodyType Rigidbody2D::GetType() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::GetTypeDef();
		}
		return Box2D_To_RigidbodyType(m_Data->Body->GetType());
	}

	void Rigidbody2D::SetType(RigidbodyType type)
	{
		Rigidbody2D::SetTypeDef(type);
		if (m_Data->Body != nullptr)
		{
			m_Data->Body->SetType(RigidbodyType_To_Box2D(type));
}
	}

	Vector2f Rigidbody2D::GetLinearVelocity() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::GetLinearVelocityDef();
		}
		b2Vec2 velocity = m_Data->Body->GetLinearVelocity();
		return Vector2f(velocity.x, velocity.y);
	}

	void Rigidbody2D::SetLinearVelocity(Vector2f velocity)
	{
		Rigidbody2D::SetLinearVelocityDef(velocity);
		if (m_Data->Body != nullptr)
		{
			m_Data->Body->SetLinearVelocity({ velocity.X, velocity.Y });
		}
	}

	float Rigidbody2D::GetAngularVelocity() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::GetAngularVelocityDef();
		}
		return m_Data->Body->GetAngularVelocity();
	}

	void Rigidbody2D::SetAngularVelocity(float velocity)
	{
		Rigidbody2D::SetAngularVelocityDef(velocity);
		if (m_Data->Body != nullptr)
		{
			m_Data->Body->SetAngularVelocity(velocity);
		}
	}

	float Rigidbody2D::GetLinearDamping() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::GetLinearDampingDef();
		}
		return m_Data->Body->GetLinearDamping();
	}

	void Rigidbody2D::SetLinearDamping(float damping)
	{
		Rigidbody2D::SetLinearDampingDef(damping);
		if (m_Data->Body != nullptr)
		{
			m_Data->Body->SetLinearDamping(damping);
		}
	}

	float Rigidbody2D::GetAngularDamping() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::GetAngularDampingDef();
		}
		return m_Data->Body->GetAngularDamping();
	}

	void Rigidbody2D::SetAngularDamping(float damping)
	{
		Rigidbody2D::SetAngularDampingDef(damping);
		if (m_Data->Body != nullptr)
		{
			m_Data->Body->SetAngularDamping(damping);
		}
	}

	float Rigidbody2D::GetGravityScale() const
	{
		if (m_Data->Body == nullptr)
		{
			return Rigidbody2D::GetGravityScaleDef();
		}
		return m_Data->Body->GetGravityScale();
	}

	void Rigidbody2D::SetGravityScale(float gravityScale)
	{
		Rigidbody2D::SetGravityScaleDef(gravityScale);
		if (m_Data->Body != nullptr)
		{
			m_Data->Body->SetGravityScale(gravityScale);
		}
	}


	b2BodyType RigidbodyType_To_Box2D(RigidbodyType type)
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

	RigidbodyType Box2D_To_RigidbodyType(b2BodyType type)
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
#else

#error No Physics 2D Engine selected

#endif
}