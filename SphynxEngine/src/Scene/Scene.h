#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Math/Transform.h"

#include "entt/entt.hpp"
#include <cstring>


namespace Sphynx
{	
	class SPHYNX_API Scene
	{
	public:
		Scene();
		Scene(std::string name);
		~Scene();

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
	};
}
