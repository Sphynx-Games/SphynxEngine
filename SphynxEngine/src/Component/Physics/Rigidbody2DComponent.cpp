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
		return m_Rigidbody->IsEnabled();
	}

	void Rigidbody2DComponent::SetEnabled(bool enabled)
	{
		Enabled = enabled;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetEnabled(enabled);
		}
	}

	RigidbodyType Rigidbody2DComponent::GetRigidbodyType()
	{
		if (m_Rigidbody == nullptr)
		{
			return Type;
		}
		return m_Rigidbody->GetType();
	}

	void Rigidbody2DComponent::SetRigidbodyType(RigidbodyType type)
	{
		Type = type;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetType(type);
		}
	}

	Vector2f Rigidbody2DComponent::GetLinearVelocity()
	{
		if (m_Rigidbody == nullptr)
		{
			return LinearVelocity;
		}
		return m_Rigidbody->GetLinearVelocity();
	}

	void Rigidbody2DComponent::SetLinearVelocity(Vector2f velocity)
	{
		LinearVelocity = velocity;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetLinearVelocity(velocity);
		}
	}

	float Rigidbody2DComponent::GetAngularVelocity()
	{
		if (m_Rigidbody == nullptr)
		{
			return AngularVelocity;
		}
		return m_Rigidbody->GetAngularVelocity();
	}

	void Rigidbody2DComponent::SetAngularVelocity(float velocity)
	{
		AngularVelocity = velocity;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetAngularVelocity(velocity);
		}
	}

	float Rigidbody2DComponent::GetLinearDamping()
	{
		if (m_Rigidbody == nullptr)
		{
			return LinearDamping;
		}
		return m_Rigidbody->GetLinearDamping();
	}

	void Rigidbody2DComponent::SetLinearDamping(float damping)
	{
		LinearDamping = damping;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetLinearDamping(damping);
		}
	}

	float Rigidbody2DComponent::GetAngularDamping()
	{
		if (m_Rigidbody == nullptr)
		{
			return AngularDamping;
		}
		return m_Rigidbody->GetAngularDamping();
	}

	void Rigidbody2DComponent::SetAngularDamping(float damping)
	{
		AngularDamping = damping;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetAngularDamping(damping);
		}
	}

	float Rigidbody2DComponent::GetGravityScale()
	{
		if (m_Rigidbody == nullptr)
		{
			return GravityScale;
		}
		return m_Rigidbody->GetGravityScale();
	}

	void Rigidbody2DComponent::SetGravityScale(float gravityScale)
	{
		GravityScale = gravityScale;
		if (m_Rigidbody != nullptr)
		{
			m_Rigidbody->SetGravityScale(gravityScale);
		}
	}
}
