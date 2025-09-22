#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Container/Map.h"


namespace Sphynx
{
	class Scene;
	class Actor;
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
		static void OnCreateAnimationComponent(void* component, Actor* actor);
		static void OnDestroyAnimationComponent(void* component, Actor* actor);

		static void OnCreateSpriteComponent(void* component, Actor* actor);
		static void OnDestroySpriteComponent(void* component, Actor* actor);

	private:
		inline static Scene* s_Scene;
		inline static HashMap<AnimationComponent*, AnimationData> s_AnimationComponents = {}; // TimeElapsed

		inline static DelegateHandle s_OnCreateAnimationHandle;
		inline static DelegateHandle s_OnDestroyAnimationHandle;
		inline static DelegateHandle s_OnCreateSpriteHandle;
		inline static DelegateHandle s_OnDestroySpriteHandle;
	};
}