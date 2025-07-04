#pragma once

#include "Sound/SoundSystem.h"

#include <fmod.hpp>


namespace Sphynx
{
	struct SoundSystemData
	{
	public:
		SoundSystemData() : System(nullptr) {}

	protected:
		FMOD::System* System;

		friend class SoundSystem;
	};

	struct SoundData
	{
	public:
		SoundData() : Sound(nullptr) {}

	protected:
		FMOD::Sound* Sound;

		friend class Sound;
		friend class SoundSystem;
	};

	struct SoundInstanceData
	{
	public:
		SoundInstanceData() : Channel(nullptr) {}

	protected:
		FMOD::Channel* Channel;

		friend class SoundInstance;
		friend class SoundSystem;
	};
}