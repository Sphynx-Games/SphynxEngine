#include "spxpch.h"
#include "SoundSystem.h"

#include "Sound.h"

#ifdef SPX_SOUND_FMOD

#include "Platform/FMOD/FMODSoundData.h"
#include <fmod_errors.h>

#else

#error No Sound Engine selected

#endif


namespace Sphynx
{
#ifdef SPX_SOUND_FMOD
	SoundSystem::SoundSystem() :
		m_Data(new SoundSystemData())
	{
		FMOD_RESULT result = FMOD::System_Create(&m_Data->System);
		SPX_CORE_ASSERT(result == FMOD_OK, "Error when creating SoundSystem!!");

		result = m_Data->System->init(512, FMOD_INIT_NORMAL, nullptr);
		SPX_CORE_ASSERT(result == FMOD_OK, "Error when initiating SoundSystem!!");
	}

	void SoundSystem::Update()
	{
		if (m_Data->System != nullptr)
		{
			m_Data->System->update();
		}
	}

	SoundInstance* SoundSystem::CreateSound(Sound* sound, SoundModeFlags flags)
	{
		if (sound->GetBufferSize() == 0) return nullptr;

		int mode = FMOD_DEFAULT | FMOD_OPENMEMORY;

		FMOD_CREATESOUNDEXINFO exinfo = {};
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = static_cast<unsigned int>(sound->GetBufferSize());

		FMOD_RESULT result = m_Data->System->createSound(static_cast<const char*>(sound->GetBuffer()), mode, &exinfo, &sound->GetData()->Sound);
		if (result != FMOD_OK)
		{
			SPX_CORE_LOG_ERROR("Error when creating the sound: {}\n", FMOD_ErrorString(result));
			return nullptr;
		}

		SoundInstance* soundInstance = new SoundInstance(sound, flags);		
		return soundInstance;
	}

	void SoundSystem::PlaySound(SoundInstance* sound, float volume)
	{
		// check if a channel is already created and free resources
		FMOD::Channel*& channel = sound->GetData()->Channel;
		if (channel != nullptr)
		{
			channel->stop();
			channel = nullptr;
		}

		// create paused channel so the sound won't start playing now
		FMOD_RESULT result = m_Data->System->playSound(sound->GetSound()->GetData()->Sound, nullptr, true, &channel);
		if (result != FMOD_OK)
		{
			SPX_CORE_LOG_ERROR("Error when trying to play the sound: {}\n", FMOD_ErrorString(result));
			return;
		}
		// set volume and start playing
		channel->setVolume(volume);
		channel->setPaused(false);
	}

	void SoundSystem::PauseSound(SoundInstance* sound)
	{
		FMOD::Channel* channel = sound->GetData()->Channel;
		if (channel == nullptr) return;

		bool isPlaying = false;
		channel->isPlaying(&isPlaying);
		if (isPlaying)
		{
			channel->setPaused(true);
		}
	}

	void SoundSystem::StopSound(SoundInstance* sound)
	{
		FMOD::Channel* channel = sound->GetData()->Channel;
		if(channel == nullptr) return;

		channel->stop();
		channel = nullptr;
	}

	SoundSystem::~SoundSystem()
	{
		if (m_Data->System != nullptr)
		{
			m_Data->System->close();
			m_Data->System->release();
			delete m_Data;
		}
	}

#else

#error No Sound Engine selected

#endif
	
}