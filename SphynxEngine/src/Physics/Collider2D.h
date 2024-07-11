#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Math/Vector.h"
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
		Collider2D(Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false);

		inline Vector2f GetOffset() const { return m_Offset; }
		inline void SetOffset(Vector2f offset) { m_Offset = offset; }

		inline bool IsTrigger() const { return m_Trigger; }
		virtual void SetTrigger(bool trigger);

		inline class PhysicsWorld2D* GetPhysicsWorld() const { return m_PhysicsWorld; }
		inline class Rigidbody2D* GetRigidbody() const { return m_Rigidbody; }

		inline struct Collider2DData* GetData() const { return m_Data; }

		inline const Set<Collider2D*>& GetOverlaps() { return m_Overlaps; }

	protected:
		virtual ~Collider2D();

		virtual void AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld, const struct Transform& transform) = 0;
		virtual void AttachToRigidbody(Rigidbody2D* rigidbody) = 0;
		void Detach();

		void BeginContact(const Contact2D& contact);
		void EndContact(const Contact2D& contact);

	public:
		Delegate<void(const Contact2D&)> OnBeginContact;
		Delegate<void(const Contact2D&)> OnEndContact;

		MulticastDelegate<void(const Contact2D&)> OnBeginOverlap;
		MulticastDelegate<void(const Contact2D&)> OnEndOverlap;
		MulticastDelegate<void(const Contact2D&)> OnHit;

	protected:
		Vector2f m_Offset;
		bool m_Trigger;
		PhysicsWorld2D* m_PhysicsWorld;
		Rigidbody2D* m_Rigidbody;
		Collider2DData* m_Data;

		Set<Collider2D*> m_Overlaps;

		friend class Physics2D;
		friend class PhysicsWorld2D;
		friend class Rigidbody2D;
	};

	class SPHYNX_API BoxCollider2D : public Collider2D
	{
	public:
		BoxCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false);

		inline Vector2f GetSize() const { return m_Size; }
		inline void SetSize(Vector2f size) { m_Size = size; }

	protected:
		virtual ~BoxCollider2D() = default;

		virtual void AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld, const struct Transform& transform) override;
		virtual void AttachToRigidbody(Rigidbody2D* rigidbody) override;

	protected:
		Vector2f m_Size;
	};

	class SPHYNX_API CircleCollider2D : public Collider2D
	{
	public:
		CircleCollider2D(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false);

		inline float GetRadius() const { return m_Radius; }
		inline void SetRadius(float radius) { m_Radius = radius; }

	protected:
		virtual ~CircleCollider2D() = default;

		virtual void AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld, const struct Transform& transform) override;
		virtual void AttachToRigidbody(Rigidbody2D* rigidbody) override;

	protected:
		float m_Radius;
	};

	class SPHYNX_API CapsuleCollider2D : public Collider2D
	{
	public:
		CapsuleCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false);
		
		inline Vector2f GetSize() const { return m_Size; }
		inline void SetSize(Vector2f size) { m_Size = size; }

	protected:
		virtual ~CapsuleCollider2D() = default;

		virtual void AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld, const Transform& transform) override;
		virtual void AttachToRigidbody(Rigidbody2D* rigidbody) override;

	protected:
		Vector2f m_Size;
	};
}