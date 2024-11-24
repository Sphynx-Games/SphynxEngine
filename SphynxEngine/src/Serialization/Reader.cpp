#include "spxpch.h"
#include "Reader.h"


namespace Sphynx
{
	void Reader::Read(bool& v) const
	{
		SPX_CORE_ASSERT(m_ReadBoolFunc != nullptr, "Read function pointer not valid!");
		m_ReadBoolFunc(m_Reader, v);
	}

	void Reader::Read(char& v) const
	{
		SPX_CORE_ASSERT(m_ReadCharFunc != nullptr, "Read function pointer not valid!");
		m_ReadCharFunc(m_Reader, v);
	}

	void Reader::Read(wchar_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadWCharFunc != nullptr, "Read function pointer not valid!");
		m_ReadWCharFunc(m_Reader, v);
	}

	void Reader::Read(int8_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadInt8Func != nullptr, "Read function pointer not valid!");
		m_ReadInt8Func(m_Reader, v);
	}

	void Reader::Read(int16_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadInt16Func != nullptr, "Read function pointer not valid!");
		m_ReadInt16Func(m_Reader, v);
	}

	void Reader::Read(int32_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadInt32Func != nullptr, "Read function pointer not valid!");
		m_ReadInt32Func(m_Reader, v);
	}

	void Reader::Read(int64_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadInt64Func != nullptr, "Read function pointer not valid!");
		m_ReadInt64Func(m_Reader, v);
	}

	void Reader::Read(uint8_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadUInt8Func != nullptr, "Read function pointer not valid!");
		m_ReadUInt8Func(m_Reader, v);
	}

	void Reader::Read(uint16_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadUInt16Func != nullptr, "Read function pointer not valid!");
		m_ReadUInt16Func(m_Reader, v);
	}

	void Reader::Read(uint32_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadUInt32Func != nullptr, "Read function pointer not valid!");
		m_ReadUInt32Func(m_Reader, v);
	}

	void Reader::Read(uint64_t& v) const
	{
		SPX_CORE_ASSERT(m_ReadUInt64Func != nullptr, "Read function pointer not valid!");
		m_ReadUInt64Func(m_Reader, v);
	}

	void Reader::Read(float& v) const
	{
		SPX_CORE_ASSERT(m_ReadFloatFunc != nullptr, "Read function pointer not valid!");
		m_ReadFloatFunc(m_Reader, v);
	}

	void Reader::Read(double& v) const
	{
		SPX_CORE_ASSERT(m_ReadDoubleFunc != nullptr, "Read function pointer not valid!");
		m_ReadDoubleFunc(m_Reader, v);
	}

	void Reader::Read(char* v) const
	{
		SPX_CORE_ASSERT(m_ReadCStrFunc != nullptr, "Read function pointer not valid!");
		m_ReadCStrFunc(m_Reader, v);
	}

	void Reader::Read(wchar_t* v) const
	{
		SPX_CORE_ASSERT(m_ReadCWStrFunc != nullptr, "Read function pointer not valid!");
		m_ReadCWStrFunc(m_Reader, v);
	}

	void Reader::Read(std::string& v) const
	{
		SPX_CORE_ASSERT(m_ReadStrFunc != nullptr, "Read function pointer not valid!");
		m_ReadStrFunc(m_Reader, v);
	}

	void Reader::Read(std::wstring& v) const
	{
		SPX_CORE_ASSERT(m_ReadWStrFunc != nullptr, "Read function pointer not valid!");
		m_ReadWStrFunc(m_Reader, v);
	}

	void Reader::Read(std::filesystem::path& v) const
	{
		std::wstring s; Read(s);
		v = s;
	}

	void Reader::Read(void* v, size_t size) const
	{
		SPX_CORE_ASSERT(m_ReadBinaryFunc != nullptr, "Read function pointer not valid!");
		m_ReadBinaryFunc(m_Reader, v, size);
	}

	size_t Reader::PushSequence() const
	{
		SPX_CORE_ASSERT(m_PushSequenceFunc != nullptr, "Read function pointer not valid!");
		return m_PushSequenceFunc(m_Reader);
	}

	void Reader::PopSequence() const
	{
		SPX_CORE_ASSERT(m_PopSequenceFunc != nullptr, "Read function pointer not valid!");
		m_PopSequenceFunc(m_Reader);
	}

	void Reader::PushIndex(size_t index) const
	{
		SPX_CORE_ASSERT(m_PushIndexFunc != nullptr, "Read function pointer not valid!");
		m_PushIndexFunc(m_Reader, index);
	}

	void Reader::PopIndex() const
	{
		SPX_CORE_ASSERT(m_PopIndexFunc != nullptr, "Read function pointer not valid!");
		m_PopIndexFunc(m_Reader);
	}

	size_t Reader::PushMap() const
	{
		SPX_CORE_ASSERT(m_PushMapFunc != nullptr, "Read function pointer not valid!");
		return m_PushMapFunc(m_Reader);
	}

	void Reader::PopMap() const
	{
		SPX_CORE_ASSERT(m_PopMapFunc != nullptr, "Read function pointer not valid!");
		m_PopMapFunc(m_Reader);
	}

	void Reader::PushKey(size_t index) const
	{
		SPX_CORE_ASSERT(m_PushKeyFunc != nullptr, "Read function pointer not valid!");
		m_PushKeyFunc(m_Reader, index);
	}

	void Reader::PopKey() const
	{
		SPX_CORE_ASSERT(m_PopKeyFunc != nullptr, "Read function pointer not valid!");
		m_PopKeyFunc(m_Reader);
	}

	void Reader::PushValue(size_t index) const
	{
		SPX_CORE_ASSERT(m_PushValueFunc != nullptr, "Read function pointer not valid!");
		m_PushValueFunc(m_Reader, index);
	}

	void Reader::PopValue() const
	{
		SPX_CORE_ASSERT(m_PopValueFunc != nullptr, "Read function pointer not valid!");
		m_PopValueFunc(m_Reader);
	}

	bool Reader::SupportsBinary() const
	{
		return m_SupportsBinary;
	}

}
