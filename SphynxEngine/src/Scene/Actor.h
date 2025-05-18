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
		inline size_t GetNumComponents() const { return m_numComponents; }

		template<typename Component, typename... Args>
		Component& AddComponent(Args&&... args);

		template<typename Component>
		void RemoveComponent();

		template<typename Component>
		Component* TryGetComponent() const;

		template<typename Component>
		Component& GetComponent() const;

		template<typename Component>
		bool HasComponent() const;

		bool IsValid() const;

	public:
		inline operator entt::entity() const { return m_EntityID; }

	private:
		entt::entity m_EntityID;
		Scene* m_Scene;
		size_t m_numComponents;

		friend class SceneSerializer;
		friend class SceneDeserializer;
	};

	template<typename Component, typename... Args>
	inline Component& Actor::AddComponent(Args&&... args)
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(!HasComponent<Component>(), "Component is already in actor");
		++m_numComponents;
		return m_Scene->m_Registry.emplace<Component>(m_EntityID, std::forward<Args>(args)...);
	}

	template<typename Component>
	inline void Actor::RemoveComponent()
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(HasComponent<Component>(), "Component is not in actor");
		--m_numComponents;
		m_Scene->m_Registry.remove<Component>(m_EntityID);
	}

	template<typename Component>
	inline Component* Actor::TryGetComponent() const
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		if (HasComponent<Component>())
		{
			return &m_Scene->m_Registry.get<Component>(m_EntityID);
		}

		return nullptr;
	}

	template<typename Component>
	inline Component& Actor::GetComponent() const
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(HasComponent<Component>(), "Component is not in actor");
		return m_Scene->m_Registry.get<Component>(m_EntityID);
	}

	template<typename Component>
	inline bool Actor::HasComponent() const
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		return m_Scene->m_Registry.all_of<Component>(m_EntityID);
	}

}
