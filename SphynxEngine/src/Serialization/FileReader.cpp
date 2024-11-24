#include "spxpch.h"
#include "FileReader.h"



namespace Sphynx
{
	FileReader::FileReader(const std::filesystem::path& filepath) :
		m_File(filepath, std::ios::binary)
	{

	}

	FileReader::~FileReader()
	{
		if (m_File.is_open())
		{
			m_File.close();
		}
	}

	void FileReader::Read(bool& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(bool));
	}

	void FileReader::Read(char& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(char));
	}

	void FileReader::Read(wchar_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(wchar_t));
	}

	void FileReader::Read(int8_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(int8_t));
	}

	void FileReader::Read(int16_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(int16_t));
	}

	void FileReader::Read(int32_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(int32_t));
	}

	void FileReader::Read(int64_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(int64_t));
	}

	void FileReader::Read(uint8_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(uint8_t));
	}

	void FileReader::Read(uint16_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(uint16_t));
	}

	void FileReader::Read(uint32_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(uint32_t));
	}

	void FileReader::Read(uint64_t& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(uint64_t));
	}

	void FileReader::Read(float& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(float));
	}

	void FileReader::Read(double& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)&v, sizeof(double));
	}

	void FileReader::Read(char* v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		size_t size{ 0 }; Read(size);
		m_File.read(v, (size + 1) * sizeof(char));
	}

	void FileReader::Read(wchar_t* v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		size_t size{ 0 }; Read(size);
		m_File.read((char*)v, (size + 1) * sizeof(wchar_t));
	}

	void FileReader::Read(std::string& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		size_t size{ 0 }; Read(size);
		v.resize(size);
		m_File.read(v.data(), (size + 1) * sizeof(typename std::string::value_type));
	}

	void FileReader::Read(std::wstring& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		size_t size{ 0 }; Read(size);
		v.resize(size);
		m_File.read((char*)v.data(), (size + 1) * sizeof(typename std::wstring::value_type));
	}

	void FileReader::Read(std::filesystem::path& v) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		std::wstring s; Read(s);
		v = s;
	}

	void FileReader::Read(void* v, size_t size) const
	{
		SPX_CORE_ASSERT(m_File.is_open(), "File is not opened!");
		SPX_CORE_ASSERT(m_File.good(), "File is not valid!");
		m_File.read((char*)v, size);
	}

	size_t FileReader::PushMap() const
	{
		size_t size{ 0 }; Read(size);
		return size;
	}

	void FileReader::PopMap() const
	{

	}

	void FileReader::PushKey(size_t index) const
	{

	}

	void FileReader::PopKey() const
	{

	}

	void FileReader::PushValue(size_t index) const
	{

	}

	void FileReader::PopValue() const
	{

	}

	size_t FileReader::PushSequence() const
	{
		size_t size{ 0 }; Read(size);
		return size;
	}

	void FileReader::PopSequence() const
	{
		
	}

	void FileReader::PushIndex(size_t index) const
	{

	}

	void FileReader::PopIndex() const
	{

	}

	bool FileReader::SupportsBinary() const
	{
		return true;
	}
}
