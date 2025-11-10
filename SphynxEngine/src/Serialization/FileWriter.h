#pragma once

#include "Core/Core.h"
#include "Writer.h"

#include <fstream>
#include <iostream>
#include <filesystem>


namespace Sphynx
{
	class SPHYNX_API FileWriter
	{
	public:
		FileWriter(std::filesystem::path filepath);
		~FileWriter();

	public:
		WRITER_COMMON_BODY;
		inline bool IsValid() const { return m_File.is_open(); }

	private:
		struct
		{
			std::ofstream m_File;
		};
	};
}
