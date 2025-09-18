#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Actor.h"
#include "Container/Array.h"

#include "Reflection/Reflection.h"

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
		bool HasBegunPlay() { return m_HasBegunPlay; }
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

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

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
	Array<std::tuple<Components*...>> Scene::GetTupledComponents() const
	{
		Array<std::tuple<Components*...>> result;
		auto view = m_Registry.view<Components...>();
		for (auto entity : view)
		{
			result.Add(std::make_tuple(const_cast<Components*>(&view.get<Components>(entity))...));
		}
		return result;
	}

}


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(Sphynx::Scene, SPHYNX_API)
SPX_REFLECT_PROPERTY(m_UUID)
SPX_REFLECT_PROPERTY(m_Name)
SPX_REFLECT_PROPERTY(m_Actors)
SPX_REFLECT_CLASS_END(Sphynx::Scene, SPHYNX_API)
