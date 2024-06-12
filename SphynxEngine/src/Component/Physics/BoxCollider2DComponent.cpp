#include "BoxCollider2DComponent.h"
#include "Physics/Physics2D.h"
#include "Physics/Collider2D.h"

namespace Sphynx
{
	BoxCollider2DComponent::BoxCollider2DComponent(Vector2f size, Vector2f offset, bool isTrigger) :
		m_Size(size),
		m_Offset(offset),
		m_Trigger(isTrigger),
		m_Collider(nullptr)
	{}

	Vector2f BoxCollider2DComponent::GetSize() const
	{
		if (m_Collider == nullptr)
		{
			return m_Size;
		}
		return m_Collider->GetSize();
	}

	void BoxCollider2DComponent::SetSize(Vector2f size)
	{
		m_Size = size;
		if (m_Collider != nullptr)
		{
			m_Collider->SetSize(size);
		}
	}

	Vector2f BoxCollider2DComponent::GetOffset() const
	{
		if (m_Collider == nullptr)
		{
			return m_Offset;
		}
		return m_Collider->GetOffset();
	}

	void BoxCollider2DComponent::SetOffset(Vector2f offset)
	{
		m_Offset = offset;
		if (m_Collider != nullptr)
		{
			m_Collider->SetOffset(offset);
		}
	}

	bool BoxCollider2DComponent::IsTrigger() const
	{
		if (m_Collider == nullptr)
		{
			return m_Trigger;
		}
		return m_Collider->IsTrigger();
	}

	void BoxCollider2DComponent::SetIsTrigger(bool trigger)
	{
		m_Trigger = trigger;
		if (m_Collider != nullptr)
		{
			m_Collider->SetTrigger(trigger);
		}
	}

	bool BoxCollider2DComponent::NeedsDebug() const
	{
		return m_Debug;
	}

	void BoxCollider2DComponent::SetDebug(bool debug)
	{
		m_Debug = debug;
	}
}
