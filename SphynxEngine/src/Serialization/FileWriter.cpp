#include "spxpch.h"
#include "FileWriter.h"


namespace Sphynx
{
	FileWriter::FileWriter(std::filesystem::path filepath) :
		m_File(filepath, std::ios::binary)
	{

	}

	FileWriter::~FileWriter()
	{
		if (m_File.is_open())
		{
			m_File.close();
		}
	}

	void FileWriter::Write(bool v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(bool));
	}

	void FileWriter::Write(double v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(double));
	}

	void FileWriter::Write(float v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(float));
	}

	void FileWriter::Write(uint64_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(uint64_t));
	}

	void FileWriter::Write(uint32_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(uint32_t));
	}

	void FileWriter::Write(uint16_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(uint16_t));
	}

	void FileWriter::Write(uint8_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(uint8_t));
	}

	void FileWriter::Write(int64_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(int64_t));
	}

	void FileWriter::Write(int32_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(int32_t));
	}

	void FileWriter::Write(int16_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(int16_t));
	}

	void FileWriter::Write(int8_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(int8_t));
	}

	void FileWriter::Write(wchar_t v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(wchar_t));
	}

	void FileWriter::Write(char v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)&v, sizeof(char));
	}

	void FileWriter::Write(const char* v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		const size_t size = strlen(v); Write(size);
		m_File.write(v, (size + 1) * sizeof(char));
	}

	void FileWriter::Write(const wchar_t* v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		const size_t size = wcslen(v); Write(size);
		m_File.write((const char*)v, (size + 1) * sizeof(wchar_t));
	}

	void FileWriter::Write(const std::string& v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		const size_t size = v.size(); Write(size);
		Write(v.data(), (size + 1) * sizeof(typename std::string::value_type));
	}

	void FileWriter::Write(const std::wstring& v)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		const size_t size = v.size(); Write(size);
		Write(v.data(), (size + 1) * sizeof(typename std::wstring::value_type));
	}

	void FileWriter::Write(const std::filesystem::path& v)
	{
		Write(v.wstring());
	}

	void FileWriter::Write(const void* v, size_t size)
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.write((const char*)v, size);
	}

	void FileWriter::PushKey()
	{
		
	}

	void FileWriter::PushValue()
	{
		
	}

	void FileWriter::PushFlow()
	{
		
	}

	void FileWriter::PushSequence()
	{
		
	}

	void FileWriter::PopSequence()
	{
		
	}

	void FileWriter::PushMap()
	{
		
	}

	void FileWriter::PopMap()
	{
		
	}

	bool FileWriter::SupportsBinary() const
	{
		return true;
	}
}
