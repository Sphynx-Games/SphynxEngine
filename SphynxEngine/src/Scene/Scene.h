#pragma once

#include "Core/Core.h"
#include "Math/Transform.h"

#include "entt/entt.hpp"


namespace Sphynx
{
	class Actor;
	class Physics2DScene;
	
	class SPHYNX_API Scene
	{
	public:
		Scene();
		~Scene();

	public:
		void BeginPlay();
		void EndPlay();
		void Update(float deltaTime);

		Actor CreateActor();
		void DestroyActor(const Actor& actor);

		const std::vector<Actor>& GetActors() const;

	private:
		void InitPhysics();

	private:
		bool m_HasBegunPlay;

		entt::registry m_Registry;
		std::vector<Actor> m_Actors;

		Physics2DScene* m_PhysicsScene;

		friend class Actor;
		friend class Physics2D;
	};
}
