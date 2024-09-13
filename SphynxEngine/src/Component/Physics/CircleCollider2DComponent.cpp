#include "spxpch.h"
#include "CircleCollider2DComponent.h"
#include "Physics/Physics2D.h"
#include "Physics/Collider2D.h"

namespace Sphynx
{
    CircleCollider2DComponent::CircleCollider2DComponent(float radius, Vector2f offset, bool isTrigger) :
		m_Radius(radius),
		m_Offset(offset),
		m_Trigger(isTrigger),
		m_Collider(nullptr)
	{}

	float CircleCollider2DComponent::GetRadius() const
    {
		if (m_Collider == nullptr)
		{
			return m_Radius;
		}
        return m_Collider->GetRadius();
    }

    void CircleCollider2DComponent::SetRadius(float radius)
    {
		m_Radius = radius;
		if (m_Collider != nullptr)
		{
			m_Collider->SetRadius(radius);
		}
    }

    Vector2f CircleCollider2DComponent::GetOffset() const
    {
		if (m_Collider == nullptr)
		{
			return m_Offset;
		}
        return m_Collider->GetOffset();
    }

    void CircleCollider2DComponent::SetOffset(Vector2f offset)
    {
		m_Offset = offset;
		if (m_Collider != nullptr)
		{
			m_Collider->SetOffset(offset);
		}
    }

    bool CircleCollider2DComponent::IsTrigger() const
    {
		if (m_Collider == nullptr)
		{
			return m_Trigger;
		}
		return m_Collider->IsTrigger();
    }

    void CircleCollider2DComponent::SetIsTrigger(bool trigger)
    {
		m_Trigger = trigger;
		if (m_Collider != nullptr)
		{
			m_Collider->SetTrigger(trigger);
		}
    }

	bool CircleCollider2DComponent::NeedsDebug() const
	{
		return m_Debug;
	}

	void CircleCollider2DComponent::SetDebug(bool debug)
	{
		m_Debug = debug;
	}
}
