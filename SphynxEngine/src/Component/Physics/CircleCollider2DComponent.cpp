#include "CircleCollider2DComponent.h"
#include "Physics/Physics2D.h"

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
        return Physics2D::GetCircleColliderRadius(m_Collider);
    }

    void CircleCollider2DComponent::SetRadius(float radius)
    {
		Radius = radius;
		if (m_Collider != nullptr)
		{
            Physics2D::SetCircleColliderRadius(m_Collider, radius);
		}
    }

    Vector2f CircleCollider2DComponent::GetOffset()
    {
		if (m_Collider == nullptr)
		{
			return Offset;
		}
        return Physics2D::GetColliderOffset(m_Collider);
    }

    void CircleCollider2DComponent::SetOffset(Vector2f offset)
    {
		Offset = offset;
		if (m_Collider != nullptr)
		{
            Physics2D::SetColliderOffset(m_Collider, offset);
		}
    }

    bool CircleCollider2DComponent::IsTrigger()
    {
		if (m_Collider == nullptr)
		{
			return Trigger;
		}
		return Physics2D::IsColliderTrigger(m_Collider);
    }

    void CircleCollider2DComponent::SetIsTrigger(bool trigger)
    {
		Trigger = trigger;
		if (m_Collider != nullptr)
		{
			Physics2D::SetColliderIsTrigger(m_Collider, trigger);
		}
    }
}
