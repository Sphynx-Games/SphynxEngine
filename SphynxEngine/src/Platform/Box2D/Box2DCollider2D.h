#pragma once

#include "Math/Vector.h"
#include "Container/Array.h"
#include "Physics/Collider2D.h"
#include "Physics/Rigidbody2D.h"
#include "Box2DRigidbody2D.h"
#include "Box2DContactListener.h"

#include <box2d/b2_fixture.h>


namespace Sphynx
{
	class Box2DBoxCollider2D : public BoxCollider2D
	{
	public:
		Box2DBoxCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			BoxCollider2D(size, offset, isTrigger, debug),
			m_Fixtures()
		{}

		~Box2DBoxCollider2D()
		{
			m_Fixtures.RemoveAll();
		};

	protected:
		virtual void BeginOverlap(Contact2D contact)
		{
			Collider2D::BeginOverlap(contact);
		}

		virtual void EndOverlap(Contact2D contact)
		{
			Collider2D::EndOverlap(contact);
		}

		virtual void BeginHit(Contact2D contact)
		{
			Collider2D::BeginHit(contact);
		}

	private:
		Array<class b2Fixture*> m_Fixtures;

		friend class Box2DRigidbody2D;
		friend class Box2DContactListener;
	};

	class Box2DCircleCollider2D : public CircleCollider2D
	{
	public:
		Box2DCircleCollider2D(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			CircleCollider2D(radius, offset, isTrigger, debug),
			m_Fixtures()
		{}

		~Box2DCircleCollider2D()
		{
			m_Fixtures.RemoveAll();
		};

	protected:
		virtual void BeginOverlap(Contact2D contact)
		{
			Collider2D::BeginOverlap(contact);
		}

		virtual void EndOverlap(Contact2D contact)
		{
			Collider2D::EndOverlap(contact);
		}

		virtual void BeginHit(Contact2D contact)
		{
			Collider2D::BeginHit(contact);
		}

	private:
		Array<b2Fixture*> m_Fixtures;

		friend class Box2DRigidbody2D;
		friend class Box2DContactListener;
	};

	class Box2DCapsuleCollider2D : public CapsuleCollider2D
	{
	public:
		Box2DCapsuleCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			CapsuleCollider2D(size, offset, isTrigger, debug),
			m_Fixtures()
		{}

		~Box2DCapsuleCollider2D()
		{
			m_Fixtures.RemoveAll();
		};

	protected:
		virtual void BeginOverlap(Contact2D contact)
		{
			Collider2D::BeginOverlap(contact);
		}

		virtual void EndOverlap(Contact2D contact)
		{
			Collider2D::EndOverlap(contact);
		}

		virtual void BeginHit(Contact2D contact)
		{
			Collider2D::BeginHit(contact);
		}

	private:
		Array<b2Fixture*> m_Fixtures;

		friend class Box2DRigidbody2D;
		friend class Box2DContactListener;
	};
}