#include "spxpch.h"
#include "CapsuleCollider2DComponent.h"
#include "Physics/Physics2D.h"
#include "Physics/Collider2D.h"

namespace Sphynx
{
	CapsuleCollider2DComponent::CapsuleCollider2DComponent(Vector2f size, Vector2f offset, bool isTrigger) :
		m_Size(size),
		m_Offset(offset),
		m_Trigger(isTrigger),
		m_Collider(nullptr)
	{}

	Vector2f CapsuleCollider2DComponent::GetSize() const
	{
		if (m_Collider == nullptr)
		{
			return m_Size;
		}
		return m_Collider->GetSize();
	}

	void CapsuleCollider2DComponent::SetSize(Vector2f size)
	{
		m_Size = size;
		if (m_Collider != nullptr)
		{
			m_Collider->SetSize(size);
		}
	}

	Vector2f CapsuleCollider2DComponent::GetOffset() const
	{
		if (m_Collider == nullptr)
		{
			return m_Offset;
		}
		return m_Collider->GetOffset();
	}

	void CapsuleCollider2DComponent::SetOffset(Vector2f offset)
	{
		m_Offset = offset;
		if (m_Collider != nullptr)
		{
			m_Collider->SetOffset(offset);
		}
	}

	bool CapsuleCollider2DComponent::IsTrigger() const
	{
		if (m_Collider == nullptr)
		{
			return m_Trigger;
		}
		return m_Collider->IsTrigger();
	}

	void CapsuleCollider2DComponent::SetIsTrigger(bool trigger)
	{
		m_Trigger = trigger;
		if (m_Collider != nullptr)
		{
			m_Collider->SetTrigger(trigger);
		}
	}

	bool CapsuleCollider2DComponent::NeedsDebug() const
	{
		return m_Debug;
	}

	void CapsuleCollider2DComponent::SetDebug(bool debug)
	{
		m_Debug = debug;
	}
}
