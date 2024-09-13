#include "spxpch.h"
#include "Collider2D.h"
#include "PhysicsWorld2D.h"
#include "Rigidbody2D.h"
#include "Physics2D.h"
#include "Math/Transform.h"

#ifdef SPX_PHYSICS_2D_BOX2D

#include "Platform/Box2D/Box2DData.h"

#include <glm/glm.hpp>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#else

#error No Physics 2D Engine selected

#endif


namespace Sphynx
{
#ifdef SPX_PHYSICS_2D_BOX2D

	struct Collider2DData
	{
	public:
		Collider2DData() : Fixtures() {};
		~Collider2DData() { Fixtures.RemoveAll(); };

	public:
		Array<b2Fixture*> Fixtures;

		friend Collider2D;
	};

	bool IsHit(Collider2D* collider1, Collider2D* collider2);
	b2Body* GetB2BodyFromPhysicsWorld(Collider2D* collider, const Transform& transform);
	b2Body* GetB2BodyFromRigidbody(Collider2D* collider);
	b2Fixture* CreateFixture(Collider2D* collider, b2Body* body, const b2Shape& shape);
	void CreateBoxFixtures(BoxCollider2D* collider, b2Body* body, const Transform& transform);
	void CreateCircleFixtures(CircleCollider2D* collider, b2Body* body, const Transform& transform);
	void CreateCapsuleFixtures(CapsuleCollider2D* collider, b2Body* body, const Transform& transform);


	Collider2D::Collider2D(Vector2f offset, bool isTrigger) :
		m_Offset(offset),
		m_Trigger(isTrigger),
		m_PhysicsWorld(nullptr),
		m_Rigidbody(nullptr),
		m_Data(new Collider2DData()),
		m_Overlaps()
	{
	}

	Collider2D::~Collider2D()
	{
		m_PhysicsWorld = nullptr;
		m_Rigidbody = nullptr;
		delete m_Data;
	}

	void Collider2D::SetTrigger(bool trigger)
	{
		m_Trigger = trigger;
		for (auto fixture : m_Data->Fixtures)
		{
			fixture->SetSensor(trigger);
		}
	}

	void Collider2D::Detach()
	{
		if (m_Data->Fixtures.Size() > 0)
		{
			Array<b2Fixture*> fixtures = m_Data->Fixtures;
			if (m_Rigidbody != nullptr)
			{
				for (b2Fixture* fix : fixtures)
				{
					fix->GetBody()->DestroyFixture(fix);
				}
				fixtures.RemoveAll();
			}
			else if(m_PhysicsWorld != nullptr)
			{
				m_PhysicsWorld->GetData()->PhysicsWorld.DestroyBody( m_Data->Fixtures[0]->GetBody() );
			}
			m_Data->Fixtures.RemoveAll();
		}
		m_PhysicsWorld = nullptr;
		m_Rigidbody = nullptr;
	}

	Collider2DData* Collider2D::GetData()
	{
		return m_Data;
	}

	void Collider2D::BeginContact(const Contact2D& contact)
	{
		//SPX_CORE_ASSERT(contact.Data.IsFromListener(), "BeginContact was not called from insede a contact listener!!");

		if (!IsHit(this, contact.OtherCollider))
		{
			m_Overlaps.Add(contact.OtherCollider);
			OnBeginOverlap.Broadcast(contact);
		}
		else
		{
			OnHit.Broadcast(contact);
		}
	}

	void Collider2D::EndContact(const Contact2D& contact)
	{
		//SPX_CORE_ASSERT(contact.Data.IsFromListener(), "EndContact was not called from insede a contact listener!!");

		if (!IsHit(this, contact.OtherCollider))
		{
			m_Overlaps.Remove(contact.OtherCollider);
			OnEndOverlap.Broadcast(contact);
		}
	}

	BoxCollider2D::BoxCollider2D(Vector2f size, Vector2f offset, bool isTrigger) :
		Collider2D(offset, isTrigger),
		m_Size(size)
	{}

	void BoxCollider2D::AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld, const Transform& transform)
	{
		m_PhysicsWorld = physicsWorld;
		b2Body* body = GetB2BodyFromPhysicsWorld(this, transform);
		CreateBoxFixtures(this, body, transform);
	}

	void BoxCollider2D::AttachToRigidbody(Rigidbody2D* rigidbody)
	{
		m_Rigidbody = rigidbody;
		b2Body* body = GetB2BodyFromRigidbody(this);
		CreateBoxFixtures(this, body, rigidbody->GetTransform());
	}

	CircleCollider2D::CircleCollider2D(float radius, Vector2f offset, bool isTrigger) :
		Collider2D(offset, isTrigger),
		m_Radius(radius)
	{}

	void CircleCollider2D::AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld, const Transform& transform)
	{
		m_PhysicsWorld = physicsWorld;
		b2Body* body = GetB2BodyFromPhysicsWorld(this, transform);
		CreateCircleFixtures(this, body, transform);
	}

	void CircleCollider2D::AttachToRigidbody(Rigidbody2D* rigidbody)
	{
		m_Rigidbody = rigidbody;
		b2Body* body = GetB2BodyFromRigidbody(this);
		CreateCircleFixtures(this, body, rigidbody->GetTransform());
	}

	CapsuleCollider2D::CapsuleCollider2D(Vector2f size, Vector2f offset, bool isTrigger) :
		Collider2D(offset, isTrigger),
		m_Size(size)
	{}

	void CapsuleCollider2D::AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld, const Transform& transform)
	{
		m_PhysicsWorld = physicsWorld;
		b2Body* body = GetB2BodyFromPhysicsWorld(this, transform);
		CreateCapsuleFixtures(this, body, transform);
	}

	void CapsuleCollider2D::AttachToRigidbody(Rigidbody2D* rigidbody)
	{
		m_Rigidbody = rigidbody;
		m_PhysicsWorld = m_Rigidbody->GetPhysicsWorld();
		b2Body* body = GetB2BodyFromRigidbody(this);
		CreateCapsuleFixtures(this, body, rigidbody->GetTransform());
	}


	bool IsHit(Collider2D* collider1, Collider2D* collider2)
	{
		return !collider1->IsTrigger() && !collider2->IsTrigger();
	}

	b2Body* GetB2BodyFromPhysicsWorld(Collider2D* collider, const Transform& transform)
	{
		b2BodyDef bodyDef;
		bodyDef.position = { transform.Position.X, transform.Position.Y };
		bodyDef.angle = glm::radians(transform.Rotation.Z);

		return collider->GetPhysicsWorld()->GetData()->PhysicsWorld.CreateBody(&bodyDef);
	}

	b2Body* GetB2BodyFromRigidbody(Collider2D* collider)
	{
		return collider->GetRigidbody()->GetData()->Body;
	}

	b2Fixture* CreateFixture(Collider2D* collider, b2Body* body, const b2Shape& shape)
	{
		b2Fixture* fixture = body->CreateFixture(&shape, 1.0f);
		fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(collider);
		return fixture;
	};

	void CreateBoxFixtures(BoxCollider2D* collider, b2Body* body, const Transform& transform)
	{
		b2PolygonShape shape;
		shape.SetAsBox(
			(collider->GetSize().X * transform.Scale.X) / 2.0f,
			(collider->GetSize().Y * transform.Scale.Y) / 2.0f,
			{ collider->GetOffset().X * transform.Scale.X, collider->GetOffset().Y * transform.Scale.Y },
			0.0f
		);

		collider->GetData()->Fixtures.Add(CreateFixture(collider, body, shape));
	}

	void CreateCircleFixtures(CircleCollider2D* collider, b2Body* body, const Transform& transform)
	{
		b2CircleShape shape;
		shape.m_p = { collider->GetOffset().X * transform.Scale.X, collider->GetOffset().Y * transform.Scale.Y };
		shape.m_radius = collider->GetRadius() * std::max(transform.Scale.X, transform.Scale.Y);

		collider->GetData()->Fixtures.Add(CreateFixture(collider, body, shape));
	}

	void CreateCapsuleFixtures(CapsuleCollider2D* collider, b2Body* body, const Transform& transform)
	{
		Vector2f capsuleSize = collider->GetSize() * Vector2f { transform.Scale.X, transform.Scale.Y };
		Vector2f scaledOffset = {
			collider->GetOffset().X * transform.Scale.X,
			collider->GetOffset().Y * transform.Scale.Y
		};

		if (capsuleSize.X == capsuleSize.Y)
		{
			// 1 CIRCLE
			b2CircleShape circle;
			circle.m_p = b2Vec2{ scaledOffset.X, scaledOffset.Y };
			circle.m_radius = capsuleSize.X / 2.0f;

			collider->GetData()->Fixtures.Add(CreateFixture(collider, body, circle));
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

			collider->GetData()->Fixtures.Add(CreateFixture(collider, body, box));
			collider->GetData()->Fixtures.Add(CreateFixture(collider, body, circleA));
			collider->GetData()->Fixtures.Add(CreateFixture(collider, body, circleB));
		}
	}

#else

#error No Physics 2D Engine selected

#endif
}