#include "spxpch.h"
#include "Sound.h"

#include <fstream>

#ifdef SPX_SOUND_FMOD

#include "Platform/FMOD/FMODSoundData.h"

#else

#error No Sound Engine selected

#endif


namespace Sphynx
{
	Sound::Sound() :
		m_Buffer(),
		m_Data(new SoundData())
	{}

	Sound::Sound(const void* buffer, size_t size) :
		m_Buffer(size),
		m_Data(new SoundData())
	{
		std::memcpy(m_Buffer.data(), buffer, size);
	}

	Sound* SoundLoader::Load(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::binary | std::ios::ate); // open file at the end

		if (!file)
		{
			SPX_CORE_LOG_ERROR("Couldn't open file at path {}", path.string().c_str());
			return nullptr;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg); // back to beginning

		std::vector<std::byte> buffer(size);
		if (!file.read((char*)buffer.data(), size))
		{
			SPX_CORE_LOG_ERROR("Couldn't read the file");
			return nullptr;
		}

		return new Sound(buffer.data(), size);
	}

#ifdef SPX_SOUND_FMOD

	Sound::~Sound()
	{
		if(m_Data->Sound != nullptr) m_Data->Sound->release();
		delete m_Data;
	}

#else

#error No Sound Engine selected

#endif

}