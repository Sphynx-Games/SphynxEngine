#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	enum SPHYNX_API SoundModeFlag : uint8_t
	{
		SOUND_3D = 1 << 0,
		SOUND_LOOP = 1 << 1,
		SOUND_STREAM = 1 << 2,
		SOUND_DEFAULT = 0,
	};

	using SoundModeFlags = typename std::underlying_type<SoundModeFlag>::type;

	class Sound;
	struct SoundInstanceData;

	class SPHYNX_API SoundInstance
	{
	public:
		SoundInstance(Sound* sound, SoundModeFlags flags = SoundModeFlag::SOUND_DEFAULT);

		~SoundInstance();

		inline Sound* GetSound() const { return m_Sound; }
		inline SoundModeFlags GetSoundModeFlags() const { return m_SoundMode; }
		void SetSoundModeFlags(SoundModeFlags flags);
		float GetVolume() const;
		void SetVolume(float volume);

		inline SoundInstanceData* GetData() const { return m_Data; }

	protected:
		Sound* m_Sound;
		SoundModeFlags m_SoundMode;

		SoundInstanceData* m_Data;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS(Sphynx::SoundInstance, SPHYNX_API)