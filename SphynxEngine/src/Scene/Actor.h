#pragma once

#include "Core/Core.h"
#include <entt/entt.hpp>


namespace Sphynx
{
	class Scene;

	class SPHYNX_API Actor
	{
	public:
		Actor();
		Actor(entt::entity entity, Scene* scene);
		virtual ~Actor();

	public:
		template<typename Component, typename... Args>
		Component& AddComponent(Args&&... args);

		template<typename Component>
		void RemoveComponent();

		template<typename Component>
		Component& GetComponent();

		template<typename Component>
		bool HasComponent();

	public:
		inline operator entt::entity() const { return m_EntityID; }

	private:
		entt::entity m_EntityID;
		Scene* m_Scene;

	};

	template<typename Component, typename... Args>
	inline Component& Actor::AddComponent(Args&&... args)
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(!HasComponent<Component>(), "Component is already in actor");
		return m_Scene->m_Registry.emplace<Component>(m_EntityID, std::forward<Args>(args)...);
	}

	template<typename Component>
	inline void Actor::RemoveComponent()
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(HasComponent<Component>(), "Component is not in actor");
		m_Scene->m_Registry.remove<Component>(m_EntityID);
	}

	template<typename Component>
	inline Component& Actor::GetComponent()
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(HasComponent<Component>(), "Component is not in actor");
		return m_Scene->m_Registry.get<Component>(m_EntityID);
	}

	template<typename Component>
	inline bool Actor::HasComponent()
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		return m_Scene->m_Registry.all_of<Component>(m_EntityID);
	}

}
