#pragma once

#include "Core/Core.h"
#include "Math/Transform.h"

#include "entt/entt.hpp"


namespace Sphynx
{	
	class SPHYNX_API Scene
	{
	public:
		Scene();
		~Scene();

	public:
		void BeginPlay();
		void EndPlay();
		void Update(float deltaTime);

		class Actor CreateActor();
		void DestroyActor(const Actor& actor);

		const std::vector<Actor>& GetActors() const;

	private:
		void InitPhysics();
		void DebugPhysics();

	private:
		bool m_HasBegunPlay;

		entt::registry m_Registry;
		std::vector<Actor> m_Actors;

		class PhysicsWorld2D* m_PhysicsWorld;

		friend class Actor;
	};
}
