#include "spxpch.h"
#include "Actor.h"

#include "Scene.h"


namespace Sphynx
{
	Actor::Actor() :
		m_EntityID(entt::null),
		m_Scene(nullptr),
		m_numComponents(0)
	{
	}

	Actor::Actor(entt::entity entity, Scene* scene) :
		m_EntityID(entity),
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
}
