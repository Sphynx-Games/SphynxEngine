#include "Box2DCollider2D.h"
#include "Physics/Rigidbody2D.h"
#include "Box2DRigidbody2D.h"
#include "Box2DContactListener.h"
#include "Math/Transform.h"

#include <glm/glm.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>


namespace Sphynx
{
	b2Fixture* CreateFixture(Box2DCollider2D* collider, b2Body* body, const b2Shape& shape)
	{
		b2Fixture* fixture = body->CreateFixture(&shape, 1.0f);
		fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(collider);
		return fixture;
	};


	Box2DCollider2D::Box2DCollider2D() : m_Fixtures()
	{
	}

	void Box2DCollider2D::DestroyFixtures(b2Body* body)
	{
		Array<b2Fixture*> fixtures = m_Fixtures;

		if (fixtures.Size() > 0)
		{
			for (b2Fixture* fix : fixtures)
			{
				body->DestroyFixture(fix);
			}
			fixtures.RemoveAll();

			//collider->m_Rigidbody = nullptr;
			//collider->SetRigidbody(nullptr);
		}
	}

	Box2DBoxCollider2D::Box2DBoxCollider2D(Vector2f size, Vector2f offset, bool isTrigger, bool debug) :
		BoxCollider2D(size, offset, isTrigger, debug),
		Box2DCollider2D()
	{}

	Box2DBoxCollider2D::~Box2DBoxCollider2D()
	{
		m_Fixtures.RemoveAll();
	}

	void Box2DBoxCollider2D::SetTrigger(bool trigger)
	{
		Collider2D::SetTrigger(trigger);
		for (auto fixture : m_Fixtures)
		{
			fixture->SetSensor(trigger);
		}
	}

	void Box2DBoxCollider2D::BeginOverlap(Contact2D contact)
	{
		Collider2D::BeginOverlap(contact);
	}

	void Box2DBoxCollider2D::EndOverlap(Contact2D contact)
	{
		Collider2D::EndOverlap(contact);
	}

	void Box2DBoxCollider2D::BeginHit(Contact2D contact)
	{
		Collider2D::BeginHit(contact);
	}

	void Box2DBoxCollider2D::CreateFixtures(b2Body* body, const Transform& transform)
	{
		b2PolygonShape shape;
		shape.SetAsBox(
			(GetSize().X * transform.Scale.X) / 2.0f,
			(GetSize().Y * transform.Scale.Y) / 2.0f,
			{ GetOffset().X * transform.Scale.X, GetOffset().Y * transform.Scale.Y },
			0.0f
		);

		m_Fixtures.Add(CreateFixture(this, body, shape));
	}

	Box2DCircleCollider2D::Box2DCircleCollider2D(float radius, Vector2f offset, bool isTrigger, bool debug) :
		CircleCollider2D(radius, offset, isTrigger, debug),
		Box2DCollider2D()
	{}

	Box2DCircleCollider2D::~Box2DCircleCollider2D()
	{
		m_Fixtures.RemoveAll();
	}

	void Box2DCircleCollider2D::SetTrigger(bool trigger)
	{
		Collider2D::SetTrigger(trigger);
		for (auto fixture : m_Fixtures)
		{
			fixture->SetSensor(trigger);
		}
	}

	void Box2DCircleCollider2D::BeginOverlap(Contact2D contact)
	{
		Collider2D::BeginOverlap(contact);
	}

	void Box2DCircleCollider2D::EndOverlap(Contact2D contact)
	{
		Collider2D::EndOverlap(contact);
	}

	void Box2DCircleCollider2D::BeginHit(Contact2D contact)
	{
		Collider2D::BeginHit(contact);
	}

	void Box2DCircleCollider2D::CreateFixtures(b2Body* body, const Transform& transform)
	{
		b2CircleShape shape;
		shape.m_p = { GetOffset().X * transform.Scale.X, GetOffset().Y * transform.Scale.Y };
		shape.m_radius = GetRadius() * std::max(transform.Scale.X, transform.Scale.Y);

		m_Fixtures.Add(CreateFixture(this, body, shape));
	}

	Box2DCapsuleCollider2D::Box2DCapsuleCollider2D(Vector2f size, Vector2f offset, bool isTrigger, bool debug) :
		CapsuleCollider2D(size, offset, isTrigger, debug),
		Box2DCollider2D()
	{}

	Box2DCapsuleCollider2D::~Box2DCapsuleCollider2D()
	{
		m_Fixtures.RemoveAll();
	}

	void Box2DCapsuleCollider2D::SetTrigger(bool trigger)
	{
		Collider2D::SetTrigger(trigger);
		for (auto fixture : m_Fixtures)
		{
			fixture->SetSensor(trigger);
		}
	}

	void Box2DCapsuleCollider2D::BeginOverlap(Contact2D contact)
	{
		Collider2D::BeginOverlap(contact);
	}

	void Box2DCapsuleCollider2D::EndOverlap(Contact2D contact)
	{
		Collider2D::EndOverlap(contact);
	}

	void Box2DCapsuleCollider2D::BeginHit(Contact2D contact)
	{
		Collider2D::BeginHit(contact);
	}

	void Box2DCapsuleCollider2D::CreateFixtures(b2Body* body, const Transform& transform)
	{
		Vector2f capsuleSize = GetSize() * Vector2f { transform.Scale.X, transform.Scale.Y };
		Vector2f scaledOffset = {
			GetOffset().X * transform.Scale.X,
			GetOffset().Y * transform.Scale.Y
		};

		if (capsuleSize.X == capsuleSize.Y)
		{
			// 1 CIRCLE
			b2CircleShape circle;
			circle.m_p = b2Vec2{ scaledOffset.X, scaledOffset.Y };
			circle.m_radius = capsuleSize.X / 2.0f;

			m_Fixtures.Add(CreateFixture(this, body, circle));
		}
		else
		{
			// 1 BOX + 2 CIRCLES
			Vector2f boxSize =
			{
				(capsuleSize.X > capsuleSize.Y) ? capsuleSize.X - capsuleSize.Y : capsuleSize.X,
				(capsuleSize.X > capsuleSize.Y) ? capsuleSize.Y : capsuleSize.Y - capsuleSize.X
			};

			b2PolygonShape box;
			box.SetAsBox(
				boxSize.X / 2.0f,
				boxSize.Y / 2.0f,
				{ scaledOffset.X, scaledOffset.Y },
				0.0f
			);

			b2CircleShape circleA; // right, up
			circleA.m_p = (capsuleSize.X > capsuleSize.Y)
				? b2Vec2{ (boxSize.X / 2.0f) + scaledOffset.X, scaledOffset.Y }
			: b2Vec2{ scaledOffset.X, (boxSize.Y / 2.0f) + scaledOffset.Y };
			circleA.m_radius = (capsuleSize.X > capsuleSize.Y) ? boxSize.Y / 2.0f : boxSize.X / 2.0f;
			b2CircleShape circleB; // left, down
			circleB.m_p = (capsuleSize.X > capsuleSize.Y)
				? b2Vec2{ (-boxSize.X / 2.0f) + scaledOffset.X, scaledOffset.Y }
			: b2Vec2{ scaledOffset.X, (-boxSize.Y / 2.0f) + scaledOffset.Y };
			circleB.m_radius = (capsuleSize.X > capsuleSize.Y) ? boxSize.Y / 2.0f : boxSize.X / 2.0f;

			m_Fixtures.Add(CreateFixture(this, body, box));
			m_Fixtures.Add(CreateFixture(this, body, circleA));
			m_Fixtures.Add(CreateFixture(this, body, circleB));
		}
	}
}