#include "spxpch.h"
#include "Actor.h"

#include "Scene.h"
#include "Component/ComponentRegistry.h"
#include "Component/UUIDComponent.h"


namespace Sphynx
{
	Actor::Actor() :
		m_EntityID(entt::null),
		m_Scene(nullptr),
		m_numComponents(0)
	{
	}

	Actor::Actor(uint32_t entityId, Scene* scene) :
		m_EntityID(entityId),
		m_Scene(scene),
		m_numComponents(0)
	{
	}

	Actor::~Actor()
	{
		m_EntityID = entt::null;
		m_Scene = nullptr;
	}

	bool Actor::IsValid() const
	{
		return m_EntityID != entt::null && m_Scene != nullptr;
	}

	void Actor::CloneComponents(const Actor& source, Actor& target)
	{
		for (const Reflection::Class* componentClass : ComponentRegistry::GetComponents())
		{
			// do not copy UUIDComponent
			if (componentClass == &Reflection::GetClass<UUIDComponent>()) continue;

			// copy the other components
			if (void* component = ComponentRegistry::InvokeGetComponent(*componentClass, source, false))
			{
				ComponentRegistry::InvokeCloneComponent(*componentClass, source, target);
			}
		}
	}
}
