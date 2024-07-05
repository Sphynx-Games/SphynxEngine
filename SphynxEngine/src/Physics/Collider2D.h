#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Math/Vector.h"
#include "Physics/Rigidbody2D.h"
#include "Physics/Physics2D.h"
#include "Container/Set.h"


namespace Sphynx
{
	struct SPHYNX_API Contact2D
	{
		class Collider2D* OtherCollider;
		Vector2f Normal;
	};


	class SPHYNX_API Collider2D
	{
	public:
		inline Vector2f GetOffset() const { return m_Offset; }
		inline void SetOffset(Vector2f offset) { m_Offset = offset; }

		inline bool IsTrigger() const { return m_Trigger; }
		inline virtual void SetTrigger(bool trigger) { m_Trigger = trigger; }

		inline class Rigidbody2D* GetRigidbody() const { return m_Rigidbody; }

		inline const Set<Collider2D*>& GetOverlaps() { return m_Overlaps; }

	protected:
		Collider2D(Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			m_Offset(offset),
			m_Trigger(isTrigger),
			m_Rigidbody(nullptr)
		{}

		virtual ~Collider2D()
		{
			m_Rigidbody = nullptr;
		};

		virtual void BeginOverlap(Contact2D contact)
		{
			m_Overlaps.Add(contact.OtherCollider);
			if (OnBeginOverlap.IsBound())
			{
				OnBeginOverlap.Execute(&contact);
			}
		}

		virtual void EndOverlap(Contact2D contact)
		{
			m_Overlaps.Remove(contact.OtherCollider);
			if (OnEndOverlap.IsBound())
			{
				OnEndOverlap.Execute(&contact);
			}
		}

		virtual void BeginHit(Contact2D contact)
		{
			if (OnHit.IsBound())
			{
				OnHit.Execute(&contact);
			}
		}

	protected:
		Vector2f m_Offset;
		bool m_Trigger;
		Rigidbody2D* m_Rigidbody;

		Set<Collider2D*> m_Overlaps;

	public:
		Delegate<void(Contact2D*)> OnBeginOverlap;
		Delegate<void(Contact2D*)> OnEndOverlap;
		Delegate<void(Contact2D*)> OnHit;

	public:
		friend class Physics2D;
		friend class Rigidbody2D;
	};

	class SPHYNX_API BoxCollider2D : public Collider2D
	{
	public:
		inline Vector2f GetSize() const { return m_Size; }
		inline void SetSize(Vector2f size) { m_Size = size; }

	protected:
		BoxCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			m_Size(size)
		{}

		virtual ~BoxCollider2D() = default;

	protected:
		Vector2f m_Size;
	};

	class SPHYNX_API CircleCollider2D : public Collider2D
	{
	public:
		inline float GetRadius() const { return m_Radius; }
		inline void SetRadius(float radius) { m_Radius = radius; }

	protected:
		CircleCollider2D(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			m_Radius(radius)
		{}

		virtual ~CircleCollider2D() = default;

	protected:
		float m_Radius;
	};

	class SPHYNX_API CapsuleCollider2D : public Collider2D
	{
	public:
		inline Vector2f GetSize() const { return m_Size; }
		inline void SetSize(Vector2f size) { m_Size = size; }

	protected:
		CapsuleCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			m_Size(size)
		{}

		virtual ~CapsuleCollider2D() = default;

	protected:
		Vector2f m_Size;
	};
}