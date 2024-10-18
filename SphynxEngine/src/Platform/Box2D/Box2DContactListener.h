#pragma once

#include "Logging/Log.h"
#include "Container/Map.h"
#include "Platform/Box2D/Box2DData.h"

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
			BeginContact(contact, contact->GetFixtureA()->GetUserData(), contact->GetFixtureB()->GetUserData());
			BeginContact(contact, contact->GetFixtureB()->GetUserData(), contact->GetFixtureA()->GetUserData());
		};

		void EndContact(b2Contact* contact)
		{
			EndContact(contact, contact->GetFixtureA()->GetUserData(), contact->GetFixtureB()->GetUserData());
			EndContact(contact, contact->GetFixtureB()->GetUserData(), contact->GetFixtureA()->GetUserData());
		}

	private:
		void BeginContact(b2Contact* contact, b2FixtureUserData& userData1, b2FixtureUserData& userData2)
		{
			Collider2D* collider1 = reinterpret_cast<Collider2D*>(userData1.pointer);
			Collider2D* collider2 = reinterpret_cast<Collider2D*>(userData2.pointer);
			if (collider1 == nullptr || collider2 == nullptr) return;

			Contact2D contactInfo = { collider2, Vector2f(contact->GetManifold()->localNormal.x, contact->GetManifold()->localNormal.y)/*, Contact2DData()*/ };

			//SPX_CORE_LOG_DISPLAY("START CONTACT!!");
			collider1->BeginContact(contactInfo);
		}

		void EndContact(b2Contact* contact, b2FixtureUserData& userData1, b2FixtureUserData& userData2)
		{
			Collider2D* collider1 = reinterpret_cast<Collider2D*>(userData1.pointer);
			Collider2D* collider2 = reinterpret_cast<Collider2D*>(userData2.pointer);
			if (collider1 == nullptr || collider2 == nullptr) return;

			Contact2D contactInfo = { collider2, Vector2f(contact->GetManifold()->localNormal.x, contact->GetManifold()->localNormal.y)/*, Contact2DData()*/ };

			//SPX_CORE_LOG_DISPLAY("END CONTACT!!");
			collider1->EndContact(contactInfo);
		}

	/*private:
		HashMap<Collider2D*, Contact2DDelegates>*/
	};
}