#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "Asset/Asset.h"


namespace Sphynx
{
	class Sound;
	class SoundInstance;

	struct SPHYNX_API SoundEmmiterComponent
	{
		COMPONENT_COMMON_BODY(SoundEmmiterComponent);
		SPX_REFLECT_GENERATED_BODY()

	public:
		SoundEmmiterComponent(AssetHandle sound, float volume = 1.0f, bool loop = false);

		void PlaySound();
		void PauseSound();
		void StopSound();

		inline AssetHandle GetSound() const { return m_Sound; }
		inline float GetVolume() const { return m_Volume; }
		inline bool GetLoop() const { return m_Loop; }

		void SetSound(AssetHandle sound);
		void SetVolume(float volume);
		void SetLoop(bool loop);

	private:
		void CreateSoundInstance();

	private:
		AssetHandle m_Sound;
		float m_Volume;
		bool m_Loop;
		SoundInstance* m_SoundInstance;
		// bool m_PlayOnAwake;
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::SoundEmmiterComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(m_Sound)
SPX_REFLECT_PROPERTY(m_Volume)
SPX_REFLECT_PROPERTY(m_Loop)

SPX_REFLECT_STRUCT_END(Sphynx::SoundEmmiterComponent, SPHYNX_API)