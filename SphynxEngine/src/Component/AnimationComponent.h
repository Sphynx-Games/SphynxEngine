#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "Container/Array.h"
#include "Asset/Asset.h"
#include "Utils/PlaybackState.h"
#include "Animation/Animation2DManager.h"


namespace Sphynx
{
	struct SPHYNX_API AnimationComponent
	{
		SPX_REFLECT_GENERATED_BODY();
		COMPONENT_COMMON_BODY(AnimationComponent);

	public:
		AnimationComponent(AssetHandle animation, float duration, bool loop = false, bool playOnBegin = false) :
			Animation(animation),
			Duration(duration),
			Loop(loop),
			m_PlaybackState(PlaybackState::STOPPED)
		{
			if (playOnBegin)
			{
				m_PlaybackState = PlaybackState::PLAYING;
			}
		}

		void Play() { Animation2DManager::PlayAnimation(this); }
		void Pause() { Animation2DManager::PauseAnimation(this); }
		void Stop() { Animation2DManager::StopAnimation(this); }

	public:
		AssetHandle Animation;
		float Duration;
		bool Loop;
		bool PlayOnBegin;

	private:
		PlaybackState m_PlaybackState;

		friend class Animation2DManager;
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::AnimationComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(Animation)
SPX_REFLECT_PROPERTY(Duration)
SPX_REFLECT_PROPERTY(Loop)
SPX_REFLECT_PROPERTY(PlayOnBegin)

SPX_REFLECT_STRUCT_END(Sphynx::AnimationComponent, SPHYNX_API)