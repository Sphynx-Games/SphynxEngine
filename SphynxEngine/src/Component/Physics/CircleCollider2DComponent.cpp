#include "CircleCollider2DComponent.h"
#include "Physics/Physics2D.h"
#include "Physics/Collider2D.h"

namespace Sphynx
{
    CircleCollider2DComponent::CircleCollider2DComponent(float radius, Vector2f offset, bool isTrigger) :
		Radius(radius),
		Offset(offset),
		Trigger(isTrigger),
		m_Collider(nullptr)
	{}

	float CircleCollider2DComponent::GetRadius()
    {
		if (m_Collider == nullptr)
		{
			return Radius;
		}
        return m_Collider->GetRadius();
    }

    void CircleCollider2DComponent::SetRadius(float radius)
    {
		Radius = radius;
		if (m_Collider != nullptr)
		{
			m_Collider->SetRadius(radius);
		}
    }

    Vector2f CircleCollider2DComponent::GetOffset()
    {
		if (m_Collider == nullptr)
		{
			return Offset;
		}
        return m_Collider->GetOffset();
    }

    void CircleCollider2DComponent::SetOffset(Vector2f offset)
    {
		Offset = offset;
		if (m_Collider != nullptr)
		{
			m_Collider->SetOffset(offset);
		}
    }

    bool CircleCollider2DComponent::IsTrigger()
    {
		if (m_Collider == nullptr)
		{
			return Trigger;
		}
		return m_Collider->IsTrigger();
    }

    void CircleCollider2DComponent::SetIsTrigger(bool trigger)
    {
		Trigger = trigger;
		if (m_Collider != nullptr)
		{
			m_Collider->SetTrigger(trigger);
		}
    }
}
