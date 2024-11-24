#include "spxpch.h"
#include "Writer.h"


namespace Sphynx
{
	void Writer::Write(bool v)
	{
		SPX_CORE_ASSERT(m_WriteBoolFunc != nullptr, "Write function pointer not valid!");
		m_WriteBoolFunc(m_Writer, v);
	}

	void Writer::Write(char v)
	{
		SPX_CORE_ASSERT(m_WriteCharFunc != nullptr, "Write function pointer not valid!");
		m_WriteCharFunc(m_Writer, v);
	}

	void Writer::Write(wchar_t v)
	{
		SPX_CORE_ASSERT(m_WriteWCharFunc != nullptr, "Write function pointer not valid!");
		m_WriteWCharFunc(m_Writer, v);
	}

	void Writer::Write(int8_t v)
	{
		SPX_CORE_ASSERT(m_WriteInt8Func != nullptr, "Write function pointer not valid!");
		m_WriteInt8Func(m_Writer, v);
	}

	void Writer::Write(int16_t v)
	{
		SPX_CORE_ASSERT(m_WriteInt16Func != nullptr, "Write function pointer not valid!");
		m_WriteInt16Func(m_Writer, v);
	}

	void Writer::Write(int32_t v)
	{
		SPX_CORE_ASSERT(m_WriteInt32Func != nullptr, "Write function pointer not valid!");
		m_WriteInt32Func(m_Writer, v);
	}

	void Writer::Write(int64_t v)
	{
		SPX_CORE_ASSERT(m_WriteInt64Func != nullptr, "Write function pointer not valid!");
		m_WriteInt64Func(m_Writer, v);
	}

	void Writer::Write(uint8_t v)
	{
		SPX_CORE_ASSERT(m_WriteUInt8Func != nullptr, "Write function pointer not valid!");
		m_WriteUInt8Func(m_Writer, v);
	}

	void Writer::Write(uint16_t v)
	{
		SPX_CORE_ASSERT(m_WriteUInt16Func != nullptr, "Write function pointer not valid!");
		m_WriteUInt16Func(m_Writer, v);
	}

	void Writer::Write(uint32_t v)
	{
		SPX_CORE_ASSERT(m_WriteUInt32Func != nullptr, "Write function pointer not valid!");
		m_WriteUInt32Func(m_Writer, v);
	}

	void Writer::Write(uint64_t v)
	{
		SPX_CORE_ASSERT(m_WriteUInt64Func != nullptr, "Write function pointer not valid!");
		m_WriteUInt64Func(m_Writer, v);
	}

	void Writer::Write(float v)
	{
		SPX_CORE_ASSERT(m_WriteFloatFunc != nullptr, "Write function pointer not valid!");
		m_WriteFloatFunc(m_Writer, v);
	}

	void Writer::Write(double v)
	{
		SPX_CORE_ASSERT(m_WriteDoubleFunc != nullptr, "Write function pointer not valid!");
		m_WriteDoubleFunc(m_Writer, v);
	}

	void Writer::Write(const char* v)
	{
		SPX_CORE_ASSERT(m_WriteCStrFunc != nullptr, "Write function pointer not valid!");
		m_WriteCStrFunc(m_Writer, v);
	}

	void Writer::Write(const wchar_t* v)
	{
		SPX_CORE_ASSERT(m_WriteCWStrFunc != nullptr, "Write function pointer not valid!");
		m_WriteCWStrFunc(m_Writer, v);
	}

	void Writer::Write(const std::string& v)
	{
		SPX_CORE_ASSERT(m_WriteStrFunc != nullptr, "Write function pointer not valid!");
		m_WriteStrFunc(m_Writer, v);
	}

	void Writer::Write(const std::wstring& v)
	{
		SPX_CORE_ASSERT(m_WriteWStrFunc != nullptr, "Write function pointer not valid!");
		m_WriteWStrFunc(m_Writer, v);
	}

	void Writer::Write(const std::filesystem::path& v)
	{
		SPX_CORE_ASSERT(m_WritePathFunc != nullptr, "Write function pointer not valid!");
		m_WritePathFunc(m_Writer, v);
	}

	void Writer::Write(const void* v, size_t size)
	{
		SPX_CORE_ASSERT(m_WriteBinaryFunc != nullptr, "Write function pointer not valid!");
		m_WriteBinaryFunc(m_Writer, v, size);
	}

	void Writer::PushMap()
	{
		SPX_CORE_ASSERT(m_PushMapFunc != nullptr, "Write function pointer not valid!");
		m_PushMapFunc(m_Writer);
	}

	void Writer::PopMap()
	{
		SPX_CORE_ASSERT(m_PopMapFunc != nullptr, "Write function pointer not valid!");
		m_PopMapFunc(m_Writer);
	}

	void Writer::PushSequence()
	{
		SPX_CORE_ASSERT(m_PushSequenceFunc != nullptr, "Write function pointer not valid!");
		m_PushSequenceFunc(m_Writer);
	}

	void Writer::PopSequence()
	{
		SPX_CORE_ASSERT(m_PopSequenceFunc != nullptr, "Write function pointer not valid!");
		m_PopSequenceFunc(m_Writer);
	}

	void Writer::PushKey()
	{
		SPX_CORE_ASSERT(m_PushKeyFunc != nullptr, "Write function pointer not valid!");
		m_PushKeyFunc(m_Writer);
	}

	void Writer::PushValue()
	{
		SPX_CORE_ASSERT(m_PushValueFunc != nullptr, "Write function pointer not valid!");
		m_PushValueFunc(m_Writer);
	}

	void Writer::PushFlow()
	{
		SPX_CORE_ASSERT(m_PushFlowFunc != nullptr, "Write function pointer not valid!");
		m_PushFlowFunc(m_Writer);
	}

	bool Writer::SupportsBinary() const
	{
		return m_SupportsBinary;
	}
}
