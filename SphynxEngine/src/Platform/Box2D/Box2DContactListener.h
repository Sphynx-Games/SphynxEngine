#pragma once

#include "Logging/Log.h"
#include "Container/Map.h"
#include "Platform/Box2D/Box2DCollider2D.h"

#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_settings.h>


namespace Sphynx
{
	class Box2DContactListener : public b2ContactListener
	{
	public:
		Box2DContactListener() = default;

		void BeginContact(b2Contact* contact) 
		{
			StartContact(contact, contact->GetFixtureA()->GetUserData(), contact->GetFixtureB()->GetUserData());
			StartContact(contact, contact->GetFixtureB()->GetUserData(), contact->GetFixtureA()->GetUserData());
		};

		void EndContact(b2Contact* contact) 
		{
			EndContact(contact, contact->GetFixtureA()->GetUserData(), contact->GetFixtureB()->GetUserData());
			EndContact(contact, contact->GetFixtureB()->GetUserData(), contact->GetFixtureA()->GetUserData());
		}

	private:
		void StartContact(b2Contact* contact, b2FixtureUserData& userData1, b2FixtureUserData& userData2)
		{
			Collider2D* collider1 = reinterpret_cast<Collider2D*>(userData1.pointer);
			Collider2D* collider2 = reinterpret_cast<Collider2D*>(userData2.pointer);
			if (collider1 == nullptr || collider2 == nullptr) return;

			Contact2D contactInfo = { collider2, Vector2f(contact->GetManifold()->localNormal.x, contact->GetManifold()->localNormal.y) };

			SPX_CORE_LOG_DISPLAY("START CONTACT!!");
			if (Box2DBoxCollider2D* colliderBox2D = dynamic_cast<Box2DBoxCollider2D*>(collider1))
			{
				if (!IsHit(collider1, collider2))
				{
					colliderBox2D->BeginOverlap(contactInfo);
				}
				else
				{
					colliderBox2D->BeginHit(contactInfo);
				}
			}
			else if (Box2DCircleCollider2D * colliderBox2D = dynamic_cast<Box2DCircleCollider2D*>(collider1))
			{
				if (!IsHit(collider1, collider2))
				{
					colliderBox2D->BeginOverlap(contactInfo);
				}
				else
				{
					colliderBox2D->BeginHit(contactInfo);
				}
			}
			else if (Box2DCapsuleCollider2D* colliderBox2D = dynamic_cast<Box2DCapsuleCollider2D*>(collider1))
			{
				if (!IsHit(collider1, collider2))
				{
					colliderBox2D->BeginOverlap(contactInfo);
				}
				else
				{
					colliderBox2D->BeginHit(contactInfo);
				}
			}
		}

		void EndContact(b2Contact* contact, b2FixtureUserData& userData1, b2FixtureUserData& userData2)
		{
			Collider2D* collider1 = reinterpret_cast<Collider2D*>(userData1.pointer);
			Collider2D* collider2 = reinterpret_cast<Collider2D*>(userData2.pointer);
			if (collider1 == nullptr || collider2 == nullptr) return;

			Contact2D contactInfo = { collider2, Vector2f(contact->GetManifold()->localNormal.x, contact->GetManifold()->localNormal.y) };

			SPX_CORE_LOG_DISPLAY("END CONTACT!!");
			if (Box2DBoxCollider2D* colliderBox2D = dynamic_cast<Box2DBoxCollider2D*>(collider1))
			{
				if (!IsHit(collider1, collider2))
				{
					colliderBox2D->EndOverlap(contactInfo);
				}
			}
			else if (Box2DCircleCollider2D* colliderBox2D = dynamic_cast<Box2DCircleCollider2D*>(collider1))
			{
				if (!IsHit(collider1, collider2))
				{
					colliderBox2D->EndOverlap(contactInfo);
				}
			}
			else if (Box2DCapsuleCollider2D* colliderBox2D = dynamic_cast<Box2DCapsuleCollider2D*>(collider1))
			{
				if (!IsHit(collider1, collider2))
				{
					colliderBox2D->EndOverlap(contactInfo);
				}
			}
		}

		bool IsHit(Collider2D* collider1, Collider2D* collider2)
		{
			return !collider1->IsTrigger() && !collider2->IsTrigger();
		}
	};
}