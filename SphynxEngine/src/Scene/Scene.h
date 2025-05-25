#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Container/Array.h"

#include "entt/entt.hpp"


namespace Sphynx
{	
	class SPHYNX_API Scene
	{
	public:
		Scene();
		Scene(std::string name);
		Scene(const Scene& other);
		~Scene();

		Scene& operator=(const Scene& other);

	public:
		void BeginPlay();
		void EndPlay();
		void Update(float deltaTime);

		class Actor& CreateActor();
		void DestroyActor(const Actor& actor);

		const std::vector<Actor>& GetActors() const;
		template<typename T>
		Array<Actor> GetActorsByComponent() const;
		template<typename T>
		Array<T*> GetComponents() const;

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

	private:
		void InitPhysics();
		void DebugPhysics();

		void CloneRegistry(const Scene& other);

	private:
		UUID m_UUID;
		std::string m_Name;

		bool m_HasBegunPlay;

		entt::registry m_Registry;
		std::vector<Actor> m_Actors;

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
			result.Emplace(entity, const_cast<Scene*>(this));
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

}
