#include "spxpch.h"
#include "SoundManager.h"

#include "Sound.h"
#include "SoundSystem.h"
#include "SoundInstance.h"


namespace Sphynx
{
	void SoundManager::Init()
	{
		s_SoundSystem = new SoundSystem();
	}

	void SoundManager::Update()
	{
		s_SoundSystem->Update();
	}

	void SoundManager::Shutdown()
	{
		delete s_SoundSystem;
	}

	SoundInstance* SoundManager::CreateSound(Sound* sound, SoundModeFlags flags)
	{
		return s_SoundSystem->CreateSound(sound, flags);
	}

	void SoundManager::PlaySound(SoundInstance* sound, float volume)
	{
		s_SoundSystem->PlaySound(sound, volume);
	}

	void SoundManager::PauseSound(SoundInstance* sound)
	{
		s_SoundSystem->PauseSound(sound);
	}

	void SoundManager::StopSound(SoundInstance* sound)
	{
		s_SoundSystem->StopSound(sound);
	}
}