#pragma once

#include "Core/Core.h"

#include "entt/entt.hpp"

namespace Sphynx
{
	class Actor;

	class SPHYNX_API Scene
	{
	public:
		Scene();
		~Scene();

	public:
		void Update(float deltaTime);

		Actor CreateActor();
		void DestroyActor(const Actor& actor);

		const std::vector<Actor>& GetActors() const;

	private:
		entt::registry m_Registry;
		std::vector<Actor> m_Actors;

	private:
		friend class Actor;

	};
}
