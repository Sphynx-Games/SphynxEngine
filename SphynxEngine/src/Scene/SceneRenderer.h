#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	class SPHYNX_API SceneRenderer
	{
	public:
		static void Render(class Scene& scene);
		static void Render(class Scene& scene, const struct Camera* camera);
		static bool GetMainCamera(Scene& scene, Camera& camera);
	};
}