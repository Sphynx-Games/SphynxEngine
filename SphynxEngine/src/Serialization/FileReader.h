#pragma once

#include "Core/Core.h"
#include "Reader.h"

#include <fstream>
#include <iostream>
#include <filesystem>


namespace Sphynx
{
	class SPHYNX_API FileReader
	{
	public:
		FileReader(const std::filesystem::path& filepath);
		~FileReader();

	public:
		READER_COMMON_BODY;
		inline bool IsValid() const { return m_File.is_open(); }

	private:
		mutable std::ifstream m_File;

	};
}