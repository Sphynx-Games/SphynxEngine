#include "BoxCollider2DComponent.h"
#include "Physics/Physics2D.h"

namespace Sphynx
{
	BoxCollider2DComponent::BoxCollider2DComponent(Vector2f size, Vector2f offset, bool isTrigger) :
		Size(size),
		Offset(offset),
		Trigger(isTrigger),
		m_Collider(nullptr)
	{}

	Vector2f BoxCollider2DComponent::GetSize()
	{
		if (m_Collider == nullptr)
		{
			return Size;
		}
		return Physics2D::GetBoxColliderSize(m_Collider);
	}

	void BoxCollider2DComponent::SetSize(Vector2f size)
	{
		Size = size;
		if (m_Collider != nullptr)
		{
			Physics2D::SetBoxColliderSize(m_Collider, size);
		}
	}

	Vector2f BoxCollider2DComponent::GetOffset()
	{
		if (m_Collider == nullptr)
		{
			return Offset;
		}
		return Physics2D::GetColliderOffset(m_Collider);
	}

	void BoxCollider2DComponent::SetOffset(Vector2f offset)
	{
		Offset = offset;
		if (m_Collider != nullptr)
		{
			Physics2D::SetColliderOffset(m_Collider, offset);
		}
	}

	bool BoxCollider2DComponent::IsTrigger()
	{
		if (m_Collider == nullptr)
		{
			return Trigger;
		}
		return Physics2D::IsColliderTrigger(m_Collider);
	}

	void BoxCollider2DComponent::SetIsTrigger(bool trigger)
	{
		Trigger = trigger;
		if (m_Collider != nullptr)
		{
			Physics2D::SetColliderIsTrigger(m_Collider, trigger);
		}
	}
}
