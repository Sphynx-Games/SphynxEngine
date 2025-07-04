#pragma once

#include "Core/Core.h"

#include <vector>


namespace Sphynx
{
	struct SoundData;

	class SPHYNX_API Sound
	{
	public:
		Sound();
		Sound(const void* buffer, size_t size);

		~Sound();

		inline const void* GetBuffer() const { return m_Buffer.data(); }
		inline size_t GetBufferSize() const { return m_Buffer.size(); }

		inline SoundData* GetData() const { return m_Data; }

	private:
		std::vector<std::byte> m_Buffer;

		SoundData* m_Data;
	};

	class SPHYNX_API SoundLoader
	{
	public:
		static Sound* Load(const std::filesystem::path& path);

	};
}

#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS(Sphynx::Sound, SPHYNX_API)