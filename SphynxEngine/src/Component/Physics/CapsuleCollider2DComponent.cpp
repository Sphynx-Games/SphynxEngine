#include "CapsuleCollider2DComponent.h"
#include "Physics/Physics2D.h"
#include "Physics/Collider2D.h"

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
		return m_Collider->GetSize();
	}

	void CapsuleCollider2DComponent::SetSize(Vector2f size)
	{
		Size = size;
		if (m_Collider != nullptr)
		{
			m_Collider->SetSize(size);
		}
	}

	Vector2f CapsuleCollider2DComponent::GetOffset()
	{
		if (m_Collider == nullptr)
		{
			return Offset;
		}
		return m_Collider->GetOffset();
	}

	void CapsuleCollider2DComponent::SetOffset(Vector2f offset)
	{
		Offset = offset;
		if (m_Collider != nullptr)
		{
			m_Collider->SetOffset(offset);
		}
	}

	bool CapsuleCollider2DComponent::IsTrigger()
	{
		if (m_Collider == nullptr)
		{
			return Trigger;
		}
		return m_Collider->IsTrigger();
	}

	void CapsuleCollider2DComponent::SetIsTrigger(bool trigger)
	{
		Trigger = trigger;
		if (m_Collider != nullptr)
		{
			m_Collider->SetTrigger(trigger);
		}
	}
}
