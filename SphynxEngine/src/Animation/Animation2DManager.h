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

	struct SPHYNX_API AnimationData
	{
		float TimeElapsed;
		SpriteRendererComponent* SpriteComponent;
	};

	class /*SPHYNX_API*/ Animation2DManager
	{
	public:
		SPHYNX_API static void Init(Scene* scene);
		SPHYNX_API static void Shutdown();

		SPHYNX_API static void Update(float deltaTime);

		SPHYNX_API static void PlayAnimation(AnimationComponent* component);
		SPHYNX_API static void PauseAnimation(AnimationComponent* component);
		SPHYNX_API static void StopAnimation(AnimationComponent* component);

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