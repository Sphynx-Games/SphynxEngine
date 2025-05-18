#pragma once

#include "Core/Core.h"

namespace Sphynx
{
	class SPHYNX_API ScriptingManager
	{
	public:
		static void Init();
		static void Shutdown();

		static void Update(const class Scene& scene, float deltaTime);
	};
}