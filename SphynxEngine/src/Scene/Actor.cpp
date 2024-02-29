#include "spxpch.h"
#include "Actor.h"

#include "Scene.h"


namespace Sphynx
{
	Actor::Actor() :
		m_EntityID(entt::null),
		m_Scene(nullptr)
	{
	}

	Actor::Actor(entt::entity entity, Scene* scene) :
		m_EntityID(entity),
		m_Scene(scene)
	{
	}

	Actor::~Actor()
	{
		m_EntityID = entt::null;
		m_Scene = nullptr;
	}
}
