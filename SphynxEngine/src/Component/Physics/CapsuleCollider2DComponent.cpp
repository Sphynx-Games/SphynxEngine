#include "CapsuleCollider2DComponent.h"
#include "Physics/Physics2D.h"

namespace Sphynx
{
	CapsuleCollider2DComponent::CapsuleCollider2DComponent(Vector2f size, Vector2f offset, bool isTrigger) :
		Size(size),
		Offset(offset),
		Trigger(isTrigger),
		m_Collider(nullptr)
	{}

	Vector2f CapsuleCollider2DComponent::GetSize()
	{
		if (m_Collider == nullptr)
		{
			return Size;
		}
		return Physics2D::GetCapsuleColliderSize(m_Collider);
	}

	void CapsuleCollider2DComponent::SetSize(Vector2f size)
	{
		Size = size;
		if (m_Collider != nullptr)
		{
			Physics2D::SetCapsuleColliderSize(m_Collider, size);
		}
	}

	Vector2f CapsuleCollider2DComponent::GetOffset()
	{
		if (m_Collider == nullptr)
		{
			return Offset;
		}
		return Physics2D::GetColliderOffset(m_Collider);
	}

	void CapsuleCollider2DComponent::SetOffset(Vector2f offset)
	{
		Offset = offset;
		if (m_Collider != nullptr)
		{
			Physics2D::SetColliderOffset(m_Collider, offset);
		}
	}

	bool CapsuleCollider2DComponent::IsTrigger()
	{
		if (m_Collider == nullptr)
		{
			return Trigger;
		}
		return Physics2D::IsColliderTrigger(m_Collider);
	}

	void CapsuleCollider2DComponent::SetIsTrigger(bool trigger)
	{
		Trigger = trigger;
		if (m_Collider != nullptr)
		{
			Physics2D::SetColliderIsTrigger(m_Collider, trigger);
		}
	}
}
