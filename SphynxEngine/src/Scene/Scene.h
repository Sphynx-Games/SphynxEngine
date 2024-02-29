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

	private:
		entt::registry m_Registry;

	private:
		friend class Actor;

	};
}
