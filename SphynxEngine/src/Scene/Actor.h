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
		Actor(uint32_t entityId, Scene* scene);
		virtual ~Actor();

	public:
		inline size_t GetNumComponents() const { return m_numComponents; }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		void RemoveComponent();

		template<typename T>
		T* TryGetComponent() const;

		template<typename T>
		T& GetComponent() const;

		template<typename T>
		bool HasComponent() const;

		bool IsValid() const;

		static void CloneComponents(const Actor& source, Actor& target);

		inline Scene* GetScene() const { return m_Scene; }

	public:
		inline operator uint32_t() const { return m_EntityID; }

	protected:
		uint32_t m_EntityID;
		Scene* m_Scene;
		size_t m_numComponents;

		friend class SceneSerializer;
		friend class SceneDeserializer;
	};

	template<typename T, typename... Args>
	inline T& Actor::AddComponent(Args&&... args)
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(!HasComponent<T>(), "Component is already in actor");
		++m_numComponents;
		T& component = m_Scene->m_Registry.emplace<T>(static_cast<entt::entity>(m_EntityID), std::forward<Args>(args)...);
		
		const Reflection::Class* reflectionClass = &Reflection::GetClass<T>();
		if (m_Scene->m_OnAddComponentDelegates.ContainsKey(reflectionClass))
		{
			m_Scene->m_OnAddComponentDelegates[reflectionClass].Broadcast(&component, this);
		}
		return component;
	}

	template<typename T>
	inline void Actor::RemoveComponent()
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(HasComponent<T>(), "Component is not in actor");
		const Reflection::Class* reflectionClass = &Reflection::GetClass<T>();
		if (m_Scene->m_OnRemoveComponentDelegates.ContainsKey(reflectionClass))
		{
			T& component = m_Scene->m_Registry.get<T>(static_cast<entt::entity>(m_EntityID));
			m_Scene->m_OnRemoveComponentDelegates[reflectionClass].Broadcast(&component, this);
		}

		--m_numComponents;
		m_Scene->m_Registry.remove<T>(static_cast<entt::entity>(m_EntityID));
	}

	template<typename T>
	inline T* Actor::TryGetComponent() const
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		if (HasComponent<T>())
		{
			return &m_Scene->m_Registry.get<T>(static_cast<entt::entity>(m_EntityID));
		}

		return nullptr;
	}

	template<typename T>
	inline T& Actor::GetComponent() const
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		SPX_CORE_ASSERT(HasComponent<T>(), "Component is not in actor");
		return m_Scene->m_Registry.get<T>(static_cast<entt::entity>(m_EntityID));
	}

	template<typename T>
	inline bool Actor::HasComponent() const
	{
		SPX_CORE_ASSERT(m_Scene != nullptr, "Actor has not a valid scene");
		return m_Scene->m_Registry.all_of<T>(static_cast<entt::entity>(m_EntityID));
	}

}


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS(Sphynx::Actor, SPHYNX_API)
