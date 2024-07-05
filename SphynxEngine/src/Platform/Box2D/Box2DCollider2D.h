#pragma once

#include "Math/Vector.h"
#include "Container/Array.h"
#include "Physics/Collider2D.h"


class b2Body;
class b2Fixture;

namespace Sphynx
{
	class Box2DCollider2D 
	{
	public:
		Box2DCollider2D();

	protected:
		virtual void CreateFixtures(class b2Body* body, const struct Transform& transform) = 0;
		void DestroyFixtures(b2Body* body);

	protected:
		Array<class b2Fixture*> m_Fixtures;

	public:
		friend class Box2DPhysicsWorld2D;
		friend class Box2DRigidbody2D;
	};

	class Box2DBoxCollider2D : public BoxCollider2D, public Box2DCollider2D
	{
	public:
		Box2DBoxCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		~Box2DBoxCollider2D();

		virtual void SetTrigger(bool trigger) override;

	protected:
		virtual void BeginOverlap(Contact2D contact) override;
		virtual void EndOverlap(Contact2D contact) override;
		virtual void BeginHit(Contact2D contact) override;

		virtual void CreateFixtures(b2Body* body, const Transform& transform) override;

	private:		
		friend class Box2DContactListener;
	};

	class Box2DCircleCollider2D : public CircleCollider2D, public Box2DCollider2D
	{
	public:
		Box2DCircleCollider2D(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		~Box2DCircleCollider2D();

		virtual void SetTrigger(bool trigger) override;

	protected:
		virtual void BeginOverlap(Contact2D contact) override;
		virtual void EndOverlap(Contact2D contact) override;
		virtual void BeginHit(Contact2D contact) override;

		virtual void CreateFixtures(b2Body* body, const Transform& transform) override;

	private:
		friend class Box2DContactListener;
	};

	class Box2DCapsuleCollider2D : public CapsuleCollider2D, public Box2DCollider2D
	{
	public:
		Box2DCapsuleCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		~Box2DCapsuleCollider2D();

		virtual void SetTrigger(bool trigger) override;

	protected:
		virtual void BeginOverlap(Contact2D contact) override;
		virtual void EndOverlap(Contact2D contact) override;
		virtual void BeginHit(Contact2D contact) override;

		virtual void CreateFixtures(b2Body* body, const Transform& transform) override;

	private:
		friend class Box2DContactListener;
	};
}