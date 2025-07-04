#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Math/Vector.h"
#include "Container/Map.h"
#include "Container/Set.h"
#include "SoundInstance.h"


namespace Sphynx
{
	class Sound;
	class SoundSystem;
	class SoundInstance;

	class SPHYNX_API SoundManager
	{
	public:
		static void Init();
		static void Update();
		static void Shutdown();

		static SoundInstance* CreateSound(Sound* sound, SoundModeFlags flags = SoundModeFlag::SOUND_DEFAULT);

		static void PlaySound(SoundInstance* sound, float volume = 1.0f);
		static void PauseSound(SoundInstance* sound);
		static void StopSound(SoundInstance* sound);

	private:
		inline static SoundSystem* s_SoundSystem;
	};
}