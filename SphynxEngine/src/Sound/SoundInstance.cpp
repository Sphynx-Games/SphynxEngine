#include "spxpch.h"
#include "SoundInstance.h"

#include "Asset/Sound/SoundAsset.h"
#include "SoundManager.h"

#ifdef SPX_SOUND_FMOD

#include "Platform/FMOD/FMODSoundData.h"

#else

#error No Sound Engine selected

#endif


namespace Sphynx
{
	SoundInstance::SoundInstance(Sound* sound, SoundModeFlags flags) :
		m_Sound(sound),
		m_SoundMode(flags),
		m_Data(new SoundInstanceData())
	{};

	SoundInstance::~SoundInstance()
	{
		m_Sound = nullptr;
		delete m_Data;
		m_Data = nullptr;
	}

#ifdef SPX_SOUND_FMOD

	void SoundInstance::SetSoundModeFlags(SoundModeFlags flags)
	{
		if (m_Data != nullptr && m_Data->Channel != nullptr)
		{
			int mode = FMOD_DEFAULT | FMOD_OPENMEMORY;
			if (flags != SoundModeFlag::SOUND_DEFAULT)
			{
				if(flags & SoundModeFlag::SOUND_3D)			mode |= FMOD_3D;
				else										mode |= FMOD_2D;
				if (flags & SoundModeFlag::SOUND_LOOP)		mode |= FMOD_LOOP_NORMAL;
				else										mode |= FMOD_LOOP_OFF;
				if (flags & SoundModeFlag::SOUND_STREAM)	mode |= FMOD_CREATESTREAM;
			}
			m_Data->Channel->setMode(mode);
		}
		m_SoundMode = flags;
	}

	float SoundInstance::GetVolume() const
	{
		if (m_Data != nullptr && m_Data->Channel != nullptr)
		{
			float volume;
			m_Data->Channel->getVolume(&volume);
			return volume;
		}
		else
		{
			return 1.0f;
		}
	}

	void SoundInstance::SetVolume(float volume)
	{
		if (m_Data != nullptr && m_Data->Channel != nullptr)
		{
			m_Data->Channel->setVolume(volume);
		}		
	}

#else

#error No Sound Engine selected

#endif
}