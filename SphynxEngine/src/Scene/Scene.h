#pragma once

#include "Core/Core.h"
#include "Math/Transform.h"

#include "entt/entt.hpp"

class b2World;
class b2Body;

namespace Sphynx
{
	class Actor;
	class RigidbodyComponent;
	
	class SPHYNX_API Scene
	{
	public:
		Scene();
		~Scene();

	public:
		void BeginPlay();
		void Update(float deltaTime);

		Actor CreateActor();
		void DestroyActor(const Actor& actor);

		const std::vector<Actor>& GetActors() const;

	private:
		void PostPhysicsUpdate(float timeStep);

	private:
		bool m_HasBegunPlay;
		entt::registry m_Registry;
		
		b2World* m_PhysicWorld;
		std::unordered_map<RigidbodyComponent*, b2Body*> m_Rigidbodies;

		std::vector<Actor> m_Actors;

	private:
		friend class Actor;

	};
}
