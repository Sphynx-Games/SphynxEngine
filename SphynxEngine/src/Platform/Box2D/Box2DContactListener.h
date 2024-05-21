#pragma once

#include "Logging/Log.h"
#include "Component/Physics/Rigidbody2DComponent.h"

#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_settings.h>
#include <unordered_map>

namespace Sphynx
{
	struct Box2DContactListener : public b2ContactListener
	{
	public:
		Box2DContactListener() /*: m_RigidbodyContacts()*/ {};

		void BeginContact(b2Contact* contact) 
		{
			StartContact(&contact->GetFixtureA()->GetBody()->GetUserData());
			StartContact(&contact->GetFixtureB()->GetBody()->GetUserData());
		};

		void EndContact(b2Contact* contact) 
		{
			EndContact(&contact->GetFixtureA()->GetBody()->GetUserData());
			EndContact(&contact->GetFixtureB()->GetBody()->GetUserData());
		}

	private:
		void StartContact(void* bodyUserData) 
		{
			if (!bodyUserData) return;

			/*RigidbodyComponent* rigidbody = static_cast<RigidbodyComponent*>(bodyUserData);
			if (HasKey(m_RigidbodyContacts, rigidbody))
			{
				uint32_t num = m_RigidbodyContacts[rigidbody];
				m_RigidbodyContacts[rigidbody] = ++num;
			}
			else
			{
				m_RigidbodyContacts[rigidbody] = 1;
			}*/

			SPX_CORE_LOG_DISPLAY("START CONTACT!!");
		}

		void EndContact(void* bodyUserData)
		{
			if (!bodyUserData) return;

			/*RigidbodyComponent* rigidbody = static_cast<RigidbodyComponent*>(bodyUserData);
			if (HasKey(m_RigidbodyContacts, rigidbody))
			{
				uint32_t num = m_RigidbodyContacts[rigidbody];
				m_RigidbodyContacts[rigidbody] = --num;
			}*/

			SPX_CORE_LOG_DISPLAY("END CONTACT!!");
		}

		/*template<typename T, typename Q, typename W>
		bool HasKey(std::unordered_map<T, Q> m, W key)
		{
			if (m.find(key) == m.end())
				return false;

			return true;
		}

	private:
		std::unordered_map<RigidbodyComponent*, uint32_t> m_RigidbodyContacts;*/
	};
}