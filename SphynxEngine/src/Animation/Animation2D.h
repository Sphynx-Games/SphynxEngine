#pragma once

#include "Core/Core.h"
#include "Container/Map.h"


namespace Sphynx
{
	class Scene;
	struct AnimationComponent;
	struct SpriteRendererComponent;

	struct AnimationData
	{
		float TimeElapsed;
		SpriteRendererComponent* SpriteComponent;
	};

	struct SPHYNX_API Animation2D
	{
	public:
		static void Init(Scene* scene);
		static void Shutdown();

		static void Update(float deltaTime);

	private:
		inline static HashMap<AnimationComponent*, AnimationData> s_AnimationComponents = {}; // TimeElapsed
	};
}