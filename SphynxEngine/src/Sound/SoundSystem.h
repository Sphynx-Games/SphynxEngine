#pragma once

#include "Core/Core.h"
#include "SoundInstance.h"


namespace Sphynx
{
	class SoundSystemData;
	class Sound;

	class SPHYNX_API SoundSystem
	{
	public:
		SoundSystem();

		void Update();

		SoundInstance* CreateSound(Sound* sound, SoundModeFlags flags = SoundModeFlag::SOUND_DEFAULT);
		void PlaySound(SoundInstance* sound, float volume = 1.0f);
		void PauseSound(SoundInstance* sound);
		void StopSound(SoundInstance* sound);

	protected:
		~SoundSystem();

	protected:
		SoundSystemData* m_Data;

		friend class SoundManager;
	};
}