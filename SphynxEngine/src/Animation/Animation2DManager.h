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

	struct SPHYNX_API Animation2DManager
	{
	public:
		static void Init(Scene* scene);
		static void Shutdown();

		static void Update(float deltaTime);

		static void PlayAnimation(AnimationComponent* component);
		static void PauseAnimation(AnimationComponent* component);
		static void StopAnimation(AnimationComponent* component);

	private:
		inline static HashMap<AnimationComponent*, AnimationData> s_AnimationComponents = {}; // TimeElapsed
	};
}