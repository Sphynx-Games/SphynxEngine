#pragma once

#include "SoundEmmiterComponent.h"
#include "Sound/SoundInstance.h"
#include "Sound/SoundManager.h"


namespace Sphynx
{
	SoundEmmiterComponent::SoundEmmiterComponent(Sound* sound, float volume, bool loop) :
		m_Sound(sound),
		m_Volume(volume),
		m_Loop(loop),
		m_SoundInstance(nullptr)
	{
		CreateSoundInstance();
	}

	void SoundEmmiterComponent::PlaySound()
	{
		SoundManager::PlaySound(m_SoundInstance, m_Volume);
	}

	void SoundEmmiterComponent::PauseSound()
	{
		SoundManager::PauseSound(m_SoundInstance);
	}

	void SoundEmmiterComponent::StopSound()
	{
		SoundManager::StopSound(m_SoundInstance);
	}

	void SoundEmmiterComponent::SetSound(Sound* sound)
	{
		if (m_Sound != sound)
		{
			StopSound();
			m_Sound = sound;
			CreateSoundInstance();
		}
	}

	void SoundEmmiterComponent::SetVolume(float volume)
	{
		volume = (volume - 0.0f) / (1.0f - 0.0f);
		if (m_Volume != volume)
		{
			m_SoundInstance->SetVolume(volume);
		}
	}

	void SoundEmmiterComponent::SetLoop(bool loop)
	{
		if (m_Loop == loop) return;

		m_Loop = loop;
		SoundModeFlags flags;
		if (m_Loop)
		{
			flags = SoundModeFlag::SOUND_LOOP;
		}
		m_SoundInstance->SetSoundModeFlags(flags);
	}

	void SoundEmmiterComponent::CreateSoundInstance()
	{
		SoundModeFlags flags;
		if (m_Loop)
		{
			flags |= SoundModeFlag::SOUND_LOOP;
		}
		m_SoundInstance = SoundManager::CreateSound(m_Sound, flags);
		m_Volume = (m_Volume - 0.0f) / (1.0f - 0.0f);
		m_SoundInstance->SetVolume(m_Volume);
	}
}