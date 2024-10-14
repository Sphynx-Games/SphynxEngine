#pragma once

#include "Core/Core.h"
#include "Reader.h"

#include <fstream>
#include <iostream>
#include <filesystem>


namespace Sphynx
{
	class SPHYNX_API FileReader : public Reader
	{
	public:
		FileReader(std::filesystem::path filepath) :
			m_File(filepath, std::ios::binary)
		{
		}

		~FileReader()
		{
			m_File.close();
		}

		bool IsValid() const { return m_File.is_open(); }

	protected:
		virtual void ReadImpl(void* data, size_t size) override
		{
			m_File.read((char*)data, size);
		}

	private:
		std::ifstream m_File;
	};
}