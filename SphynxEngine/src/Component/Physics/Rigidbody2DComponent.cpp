#include "Rigidbody2DComponent.h"
#include "Physics/Physics2D.h"

namespace Sphynx
{
	Rigidbody2DComponent::Rigidbody2DComponent() :
		Enabled(true),
		Type(RigidbodyType::STATIC),
		LinearVelocity(Vector2f(0.0f, 0.0f)),
		AngularVelocity(0.0f),
		LinearDamping(0.0f),
		AngularDamping(0.0f),
		GravityScale(1.0f),
		m_Rigidbody(nullptr)
	{}

	bool Rigidbody2DComponent::IsEnabled()
	{
		if (m_Rigidbody == nullptr)
		{
			return Enabled;
		}
		return Physics2D::IsRigidbodyEnabled(m_Rigidbody);
	}

	void Rigidbody2DComponent::SetEnabled(bool enabled)
	{
		Enabled = enabled;
		if (m_Rigidbody != nullptr)
		{
			Physics2D::SetRigidbodyEnabled(m_Rigidbody, enabled);
		}
	}

	RigidbodyType Rigidbody2DComponent::GetRigidbodyType()
	{
		if (m_Rigidbody == nullptr)
		{
			return Type;
		}
		return Physics2D::GetRigidbodyType(m_Rigidbody);
	}

	void Rigidbody2DComponent::SetRigidbodyType(RigidbodyType type)
	{
		Type = type;
		if (m_Rigidbody != nullptr)
		{
			Physics2D::SetRigidbodyType(m_Rigidbody, type);
		}
	}

	Vector2f Rigidbody2DComponent::GetLinearVelocity()
	{
		if (m_Rigidbody == nullptr)
		{
			return LinearVelocity;
		}
		return Physics2D::GetRigidbodyLinearVelocity(m_Rigidbody);
	}

	void Rigidbody2DComponent::SetLinearVelocity(Vector2f velocity)
	{
		LinearVelocity = velocity;
		if (m_Rigidbody != nullptr)
		{
			Physics2D::SetRigidbodyLinearVelocity(m_Rigidbody, velocity);
		}
	}

	float Rigidbody2DComponent::GetAngularVelocity()
	{
		if (m_Rigidbody == nullptr)
		{
			return AngularVelocity;
		}
		return Physics2D::GetRigidbodyAngularVelocity(m_Rigidbody);
	}

	void Rigidbody2DComponent::SetAngularVelocity(float velocity)
	{
		AngularVelocity = velocity;
		if (m_Rigidbody != nullptr)
		{
			Physics2D::SetRigidbodyAngularVelocity(m_Rigidbody, velocity);
		}
	}

	float Rigidbody2DComponent::GetLinearDamping()
	{
		if (m_Rigidbody == nullptr)
		{
			return LinearDamping;
		}
		return Physics2D::GetRigidbodyLinearDamping(m_Rigidbody);
	}

	void Rigidbody2DComponent::SetLinearDamping(float damping)
	{
		LinearDamping = damping;
		if (m_Rigidbody != nullptr)
		{
			Physics2D::SetRigidbodyLinearDamping(m_Rigidbody, damping);
		}
	}

	float Rigidbody2DComponent::GetAngularDamping()
	{
		if (m_Rigidbody == nullptr)
		{
			return AngularDamping;
		}
		return Physics2D::GetRigidbodyAngularDamping(m_Rigidbody);
	}

	void Rigidbody2DComponent::SetAngularDamping(float damping)
	{
		AngularDamping = damping;
		if (m_Rigidbody != nullptr)
		{
			Physics2D::SetRigidbodyAngularDamping(m_Rigidbody, damping);
		}
	}

	float Rigidbody2DComponent::GetGravityScale()
	{
		if (m_Rigidbody == nullptr)
		{
			return GravityScale;
		}
		return Physics2D::GetRigidbodyGravityScale(m_Rigidbody);
	}

	void Rigidbody2DComponent::SetGravityScale(float gravityScale)
	{
		GravityScale = gravityScale;
		if (m_Rigidbody != nullptr)
		{
			Physics2D::SetRigidbodyGravityScale(m_Rigidbody, gravityScale);
		}
	}
}
