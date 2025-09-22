#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Actor.h"
#include "Container/Array.h"
#include "Container/Map.h"
#include "Core/Delegate.h"
#include "Reflection/Class.h"

#include "entt/entt.hpp"


namespace Sphynx
{	
	class SPHYNX_API Scene
	{
		SPX_REFLECT_GENERATED_BODY()

	public:
		Scene();
		Scene(std::string name);
		Scene(const Scene& other);
		virtual ~Scene();

		Scene& operator=(const Scene& other);

	public:
		void BeginPlay();
		inline bool HasBegunPlay() { return m_HasBegunPlay; }
		void EndPlay();
		void Update(float deltaTime);

		class Actor& CreateActor();
		void DestroyActor(const Actor& actor);

		const Array<Actor>& GetActors() const;
		template<typename T>
		Array<Actor> GetActorsByComponent() const;
		template<typename T>
		Array<T*> GetComponents() const;
		template <typename... Components>
		Array<std::tuple<Components*...>> GetTupledComponents() const;

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		template<typename T, typename Func>
		DelegateHandle SubscribeOnAddComponent(const Func& f);
		template<typename T, typename UMethod, typename U>
		DelegateHandle SubscribeOnAddComponent(U* object, UMethod method);
		template<typename T>
		void UnsubscribeOnAddComponent(DelegateHandle handle);

		template<typename T, typename Func>
		DelegateHandle SubscribeOnRemoveComponent(const Func& f);
		template<typename T, typename UMethod, typename U>
		DelegateHandle SubscribeOnRemoveComponent(U* object, UMethod method);
		template<typename T>
		void UnsubscribeOnRemoveComponent(DelegateHandle handle);

	private:
		void InitPhysics();
		void DebugPhysics();

		void CloneRegistry(const Scene& other);

	protected:
		UUID m_UUID;
		std::string m_Name;

		bool m_HasBegunPlay;

		entt::registry m_Registry;
		Array<Actor> m_Actors;

		class PhysicsWorld2D* m_PhysicsWorld;

		HashMap<const Reflection::Class*, MulticastDelegate<void(void*, Actor*)>> m_OnAddComponentDelegates;
		HashMap<const Reflection::Class*, MulticastDelegate<void(void*, Actor*)>> m_OnRemoveComponentDelegates;

		friend class Actor;
		friend class SceneSerializer;
		friend class SceneDeserializer;
		friend class SceneRenderer;
	};

	template<typename T>
	inline Array<Actor> Scene::GetActorsByComponent() const
	{
		Array<Actor> result;
		auto view = m_Registry.view<T>();
		for (auto& entity : view)
		{
			result.Emplace(static_cast<uint32_t>(entity), const_cast<Scene*>(this));
		}
		return result;
	}

	template<typename T>
	inline Array<T*> Scene::GetComponents() const
	{
		Array<T*> result;
		auto view = m_Registry.view<T>();
		for (auto entity : view)
		{
			auto& component = view.get<T>(entity);
			result.Add(const_cast<T*>(&component));
		}
		return result;
	}

	template <typename... Components>
	inline Array<std::tuple<Components*...>> Scene::GetTupledComponents() const
	{
		Array<std::tuple<Components*...>> result;
		auto view = m_Registry.view<Components...>();
		for (auto entity : view)
		{
			result.Add(std::make_tuple(const_cast<Components*>(&view.get<Components>(entity))...));
		}
		return result;
	}

	template<typename T, typename Func>
	inline DelegateHandle Scene::SubscribeOnAddComponent(const Func& f)
	{
		const Reflection::Class* reflectionClass = &Reflection::GetClass<T>();
		if (!m_OnAddComponentDelegates.ContainsKey(reflectionClass))
		{
			m_OnAddComponentDelegates.Add(reflectionClass, MulticastDelegate<void(void*, Actor*)>());
		}
		return m_OnAddComponentDelegates[reflectionClass].Bind(f);
	}

	template<typename T, typename UMethod, typename U>
	inline DelegateHandle Scene::SubscribeOnAddComponent(U* object, UMethod method)
	{
		const Reflection::Class* reflectionClass = &Reflection::GetClass<T>();
		if (!m_OnAddComponentDelegates.ContainsKey(reflectionClass))
		{
			m_OnAddComponentDelegates.Add(reflectionClass, MulticastDelegate<void(void*, Actor*)>());
		}
		return m_OnAddComponentDelegates[reflectionClass].Bind(object, method);
	}

	template<typename T>
	inline void Scene::UnsubscribeOnAddComponent(DelegateHandle handle)
	{
		const Reflection::Class* reflectionClass = &Reflection::GetClass<T>();
		SPX_CORE_ASSERT(m_OnAddComponentDelegates.ContainsKey(reflectionClass), "Class not present!!");
		m_OnAddComponentDelegates[reflectionClass].Unbind(handle);
	}

	template<typename T, typename Func>
	inline DelegateHandle Scene::SubscribeOnRemoveComponent(const Func& f)
	{
		const Reflection::Class* reflectionClass = &Reflection::GetClass<T>();
		if (!m_OnRemoveComponentDelegates.ContainsKey(reflectionClass))
		{
			m_OnRemoveComponentDelegates.Add(reflectionClass, MulticastDelegate<void(void*, Actor*)>());
		}
		return m_OnRemoveComponentDelegates[reflectionClass].Bind(f);
	}

	template<typename T, typename UMethod, typename U>
	inline DelegateHandle Scene::SubscribeOnRemoveComponent(U* object, UMethod method)
	{
		const Reflection::Class* reflectionClass = &Reflection::GetClass<T>();
		if (!m_OnRemoveComponentDelegates.ContainsKey(reflectionClass))
		{
			m_OnRemoveComponentDelegates.Add(reflectionClass, MulticastDelegate<void(void*, Actor*)>());
		}
		return m_OnRemoveComponentDelegates[reflectionClass].Bind(object, method);
	}

	template<typename T>
	inline void Scene::UnsubscribeOnRemoveComponent(DelegateHandle handle)
	{
		const Reflection::Class* reflectionClass = &Reflection::GetClass<T>();
		SPX_CORE_ASSERT(m_OnRemoveComponentDelegates.ContainsKey(reflectionClass), "Class not present!!");
		m_OnRemoveComponentDelegates[reflectionClass].Unbind(handle);
	}

}


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(Sphynx::Scene, SPHYNX_API)
SPX_REFLECT_PROPERTY(m_UUID)
SPX_REFLECT_PROPERTY(m_Name)
SPX_REFLECT_PROPERTY(m_Actors)
SPX_REFLECT_CLASS_END(Sphynx::Scene, SPHYNX_API)
