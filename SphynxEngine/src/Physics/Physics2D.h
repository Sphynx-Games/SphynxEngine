#pragma once

#include "Core/Core.h"

namespace Sphynx
{
	extern class Physics2DScene;
	class Scene;

	class SPHYNX_API Physics2D
	{
	public:
		//void Init();
		//void Shutdown();

		static Physics2DScene* CreatePhysics2DScene();
		static Physics2DScene* CreatePhysics2DScene(Scene& scene);
		static void DestroyPhysics2DScene(Physics2DScene* physicsScene);

		static void Step(Physics2DScene* physicsScene, float timeStep);
		static void Step(Scene& scene, float timeStep);

	private:
		static void PostStep(Scene& scene, float timeStep);
	};
}