#pragma once

#include "Core/Core.h"
#include "Writer.h"

#include <fstream>
#include <iostream>
#include <filesystem>


namespace Sphynx
{
	class SPHYNX_API FileWriter : public Writer
	{
	public:
		FileWriter(std::filesystem::path filepath) :
			m_File(filepath, std::ios::binary)
		{
		}

		~FileWriter()
		{
			m_File.close();
		}

	protected:
		virtual void WriteImpl(const void* data, size_t size) override
		{
			m_File.write((const char*)data, size);
		}

	private:
		std::ofstream m_File;
	};
}