#pragma once


#include "Core/Core.h"
#include "Serialization/Writer.h"

#include <fstream>
#include <iostream>


namespace YAML { class Emitter; }

namespace Sphynx
{
	class SPHYNX_API YAMLWriter
	{
	public:
		YAMLWriter(const std::filesystem::path& path);
		~YAMLWriter();

	public:
		WRITER_COMMON_BODY;
		inline bool IsValid() const { return m_File.is_open(); }

	protected:
		std::ofstream m_File;
		std::unique_ptr<YAML::Emitter> m_Emitter;

	};
}