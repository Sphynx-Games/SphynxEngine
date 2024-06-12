#include "Rigidbody2DComponent.h"
#include "Physics/Physics2D.h"

namespace Sphynx
{
	Rigidbody2DComponent::Rigidbody2DComponent() :
		m_Enabled(true),
		m_Type(RigidbodyType::STATIC),
		m_LinearVelocity(Vector2f(0.0f, 0.0f)),
		m_AngularVelocity(0.0f),
		m_LinearDamping(0.0f),
		m_AngularDamping(0.0f),
		m_GravityScale(1.0f),
		m_Rigidbody(nullptr)
	{}

	bool Rigidbody2DComponent::IsEnabled() const
	{
		if (m_Rigidbody == nullptr)
		{
			return m_Enabled;
		}
		return m_Rigidbody->IsEnabled();
	}

	void Rigidbody2DComponent::SetEnabled(bool enabled)
	{
		m_Enabled = enabled;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetEnabled(enabled);
		}
	}

	RigidbodyType Rigidbody2DComponent::GetRigidbodyType() const
	{
		if (m_Rigidbody == nullptr)
		{
			return m_Type;
		}
		return m_Rigidbody->GetType();
	}

	void Rigidbody2DComponent::SetRigidbodyType(RigidbodyType type)
	{
		m_Type = type;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetType(type);
		}
	}

	Vector2f Rigidbody2DComponent::GetLinearVelocity() const
	{
		if (m_Rigidbody == nullptr)
		{
			return m_LinearVelocity;
		}
		return m_Rigidbody->GetLinearVelocity();
	}

	void Rigidbody2DComponent::SetLinearVelocity(Vector2f velocity)
	{
		m_LinearVelocity = velocity;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetLinearVelocity(velocity);
		}
	}

	float Rigidbody2DComponent::GetAngularVelocity() const
	{
		if (m_Rigidbody == nullptr)
		{
			return m_AngularVelocity;
		}
		return m_Rigidbody->GetAngularVelocity();
	}

	void Rigidbody2DComponent::SetAngularVelocity(float velocity)
	{
		m_AngularVelocity = velocity;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetAngularVelocity(velocity);
		}
	}

	float Rigidbody2DComponent::GetLinearDamping() const
	{
		if (m_Rigidbody == nullptr)
		{
			return m_LinearDamping;
		}
		return m_Rigidbody->GetLinearDamping();
	}

	void Rigidbody2DComponent::SetLinearDamping(float damping)
	{
		m_LinearDamping = damping;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetLinearDamping(damping);
		}
	}

	float Rigidbody2DComponent::GetAngularDamping() const
	{
		if (m_Rigidbody == nullptr)
		{
			return m_AngularDamping;
		}
		return m_Rigidbody->GetAngularDamping();
	}

	void Rigidbody2DComponent::SetAngularDamping(float damping)
	{
		m_AngularDamping = damping;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetAngularDamping(damping);
		}
	}

	float Rigidbody2DComponent::GetGravityScale() const
	{
		if (m_Rigidbody == nullptr)
		{
			return m_GravityScale;
		}
		return m_Rigidbody->GetGravityScale();
	}

	void Rigidbody2DComponent::SetGravityScale(float gravityScale)
	{
		m_GravityScale = gravityScale;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetGravityScale(gravityScale);
		}
	}
}
