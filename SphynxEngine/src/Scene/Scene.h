#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Math/Transform.h"
#include "Component/UUIDComponent.h"

#include "entt/entt.hpp"
#include <cstring>


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

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

	private:
		void InitPhysics();
		void DebugPhysics();

		void CloneRegistry(const Scene& other);

		template<typename T>
		static void CopyComponent(const entt::registry& sourceRegistry, entt::registry& targetRegistry, const std::unordered_map<UUID, entt::entity>& enttMap)
		{
			auto idView = sourceRegistry.view<T>();
			for (auto& e : idView)
			{
				UUID uuid = sourceRegistry.get<UUIDComponent>(e).UUID;
				entt::entity targetEntityUUID = enttMap.at(uuid);
				const T& component = sourceRegistry.get<T>(e);
				targetRegistry.emplace_or_replace<T>(targetEntityUUID, component);
			}
		}

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
}
