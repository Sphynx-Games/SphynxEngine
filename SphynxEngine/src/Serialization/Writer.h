#pragma once


#include "Core/Core.h"

#include "Serialization/Serialization.h"
#include "Traits/Traits.h"

#include <cstring>

#define WRITER_COMMON_BODY \
	void Write(bool v); \
	void Write(char v); \
	void Write(wchar_t v); \
	void Write(int8_t v); \
	void Write(int16_t v); \
	void Write(int32_t v); \
	void Write(int64_t v); \
	void Write(uint8_t v); \
	void Write(uint16_t v); \
	void Write(uint32_t v); \
	void Write(uint64_t v); \
	void Write(float v); \
	void Write(double v); \
	 \
	void Write(const char* v); \
	void Write(const wchar_t* v); \
	void Write(const std::string& v); \
	void Write(const std::wstring& v); \
	void Write(const std::filesystem::path& v); \
	 \
	void Write(const void* v, size_t size); \
	 \
	template<typename T> \
	void Write(const T& v) { ::Sphynx::Serialization::Write(*this, v); } \
	 \
	template<typename TKey, typename TValue> \
	void Write(const TKey& k, const TValue& v) { PushKey(); Write(k); PushValue(); Write(v); } \
	 \
	void PushKey(); \
	void PushValue(); \
	void PushFlow(); \
	void PushSequence(); \
	void PopSequence(); \
	void PushMap(); \
	void PopMap(); \
	 \
	bool SupportsBinary() const


namespace Sphynx
{
	class SPHYNX_API Writer
	{
	public:
		Writer() = delete;
		Writer(Writer&& other) = delete;
		Writer& operator=(Writer&& other) = delete;

		template<typename TWriter>
		Writer(TWriter& writer) :
			m_Writer(&writer),
			m_WriteBoolFunc([](void* writer, bool v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteCharFunc([](void* writer, char v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteWCharFunc([](void* writer, wchar_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteInt8Func([](void* writer, int8_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteInt16Func([](void* writer, int16_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteInt32Func([](void* writer, int32_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteInt64Func([](void* writer, int64_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteUInt8Func([](void* writer, uint8_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteUInt16Func([](void* writer, uint16_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteUInt32Func([](void* writer, uint32_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteUInt64Func([](void* writer, uint64_t v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteFloatFunc([](void* writer, float v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteDoubleFunc([](void* writer, double v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteCStrFunc([](void* writer, const char* v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteCWStrFunc([](void* writer, const wchar_t* v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteStrFunc([](void* writer, const std::string& v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteWStrFunc([](void* writer, const std::wstring& v) { static_cast<TWriter*>(writer)->Write(v); }),
			m_WriteBinaryFunc([](void* writer, const void* v, size_t size) { static_cast<TWriter*>(writer)->Write(v, size); }),
			m_PushMapFunc([](void* writer) { static_cast<TWriter*>(writer)->PushMap(); }),
			m_PopMapFunc([](void* writer) { static_cast<TWriter*>(writer)->PopMap(); }),
			m_PushKeyFunc([](void* writer) { static_cast<TWriter*>(writer)->PushKey(); }),
			m_PushValueFunc([](void* writer) { static_cast<TWriter*>(writer)->PushValue(); }),
			m_PushSequenceFunc([](void* writer) { static_cast<TWriter*>(writer)->PushSequence(); }),
			m_PopSequenceFunc([](void* writer) { static_cast<TWriter*>(writer)->PopSequence(); }),
			m_PushFlowFunc([](void* writer) { static_cast<TWriter*>(writer)->PushFlow(); }),
			m_SupportsBinary(writer.SupportsBinary())
		{

		}

		template<typename TWriter>
		static Writer Create(const TWriter& writer) { return Writer{ writer }; }

	public:
		WRITER_COMMON_BODY;

	private:
		void* m_Writer;

		void (*m_WriteBoolFunc)(void*, bool);
		void (*m_WriteCharFunc)(void*, char);
		void (*m_WriteWCharFunc)(void*, wchar_t);
		void (*m_WriteInt8Func)(void*, int8_t);
		void (*m_WriteInt16Func)(void*, int16_t);
		void (*m_WriteInt32Func)(void*, int32_t);
		void (*m_WriteInt64Func)(void*, int64_t);
		void (*m_WriteUInt8Func)(void*, uint8_t);
		void (*m_WriteUInt16Func)(void*, uint16_t);
		void (*m_WriteUInt32Func)(void*, uint32_t);
		void (*m_WriteUInt64Func)(void*, uint64_t);
		void (*m_WriteFloatFunc)(void*, float);
		void (*m_WriteDoubleFunc)(void*, double);
		void (*m_WriteCStrFunc)(void*, const char*);
		void (*m_WriteCWStrFunc)(void*, const wchar_t*);
		void (*m_WriteStrFunc)(void*, const std::string&);
		void (*m_WriteWStrFunc)(void*, const std::wstring&);
		void (*m_WriteBinaryFunc)(void*, const void*, size_t);

		void (*m_PushMapFunc)(void*);
		void (*m_PopMapFunc)(void*);
		void (*m_PushKeyFunc)(void*);
		void (*m_PushValueFunc)(void*);
		void (*m_PushSequenceFunc)(void*);
		void (*m_PopSequenceFunc)(void*);
		void (*m_PushFlowFunc)(void*);

		bool m_SupportsBinary;

	};
}