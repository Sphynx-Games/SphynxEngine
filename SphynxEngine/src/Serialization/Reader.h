#pragma once

#include "Core/Core.h"
#include <cstdint>
#include <string>
#include <filesystem>


#define READER_COMMON_BODY \
	void Read(bool& v) const; \
	void Read(char& v) const; \
	void Read(wchar_t& v) const; \
	void Read(int8_t& v) const; \
	void Read(int16_t& v) const; \
	void Read(int32_t& v) const; \
	void Read(int64_t& v) const; \
	void Read(uint8_t& v) const; \
	void Read(uint16_t& v) const; \
	void Read(uint32_t& v) const; \
	void Read(uint64_t& v) const; \
	void Read(float& v) const; \
	void Read(double& v) const; \
	 \
	void Read(char* v) const; \
	void Read(wchar_t* v) const; \
	void Read(std::string& v) const; \
	void Read(std::wstring& v) const; \
	void Read(std::filesystem::path& v) const; \
	 \
	void Read(void* v, size_t size) const; \
	 \
	template<typename T> \
	void Read(T& v) const { ::Sphynx::Serialization::Read(*this, v); } \
	 \
	template<typename TKey, typename TValue> \
	void Read(size_t index, TKey& k, TValue& v) const { PushKey(index); Read(k); PopKey(); PushValue(index); Read(v); PopValue(); } \
	 \
	size_t PushSequence() const; \
	void PopSequence() const; \
	void PushIndex(size_t index) const; \
	void PopIndex() const; \
	\
	bool IsMap() const; \
	size_t PushMap() const; \
	void PopMap() const; \
	void PushKey(size_t index) const; \
	void PopKey() const; \
	void PushValue(size_t index) const; \
	void PopValue() const; \
	bool FindKey(const char* key, size_t& index) const; \
	bool IsNull() const; \
	\
	bool SupportsBinary() const


namespace Sphynx
{
	class SPHYNX_API Reader
	{
	public:
		Reader() = delete;
		Reader(Reader&& other) = delete;
		Reader& operator=(Reader&& other) = delete;

		template<typename TReader, 
			typename = std::enable_if_t<!std::is_same_v<
			std::remove_cv_t<std::remove_reference_t<TReader>>, 
			Reader>>>
		Reader(TReader& reader) :
			m_Reader(&reader),
			m_ReadBoolFunc([](void* reader, bool& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadCharFunc([](void* reader, char& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadWCharFunc([](void* reader, wchar_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadInt8Func([](void* reader, int8_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadInt16Func([](void* reader, int16_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadInt32Func([](void* reader, int32_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadInt64Func([](void* reader, int64_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadUInt8Func([](void* reader, uint8_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadUInt16Func([](void* reader, uint16_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadUInt32Func([](void* reader, uint32_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadUInt64Func([](void* reader, uint64_t& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadFloatFunc([](void* reader, float& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadDoubleFunc([](void* reader, double& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadCStrFunc([](void* reader, char* v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadCWStrFunc([](void* reader, wchar_t* v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadStrFunc([](void* reader, std::string& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadWStrFunc([](void* reader, std::wstring& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadPathFunc([](void* reader, std::filesystem::path& v) { static_cast<TReader*>(reader)->Read(v); }),
			m_ReadBinaryFunc([](void* reader, void* v, size_t size) { static_cast<TReader*>(reader)->Read(v, size); }),
			m_IsMapFunc([](void* reader) { return static_cast<TReader*>(reader)->IsMap(); }),
			m_PushMapFunc([](void* reader) { return static_cast<TReader*>(reader)->PushMap(); }),
			m_PopMapFunc([](void* reader) { static_cast<TReader*>(reader)->PopMap(); }),
			m_PushKeyFunc([](void* reader, size_t index) { static_cast<TReader*>(reader)->PushKey(index); }),
			m_PopKeyFunc([](void* reader) { static_cast<TReader*>(reader)->PopKey(); }),
			m_PushValueFunc([](void* reader, size_t index) { static_cast<TReader*>(reader)->PushValue(index); }),
			m_PopValueFunc([](void* reader) { static_cast<TReader*>(reader)->PopValue(); }),
			m_PushSequenceFunc([](void* reader) { return static_cast<TReader*>(reader)->PushSequence(); }),
			m_PopSequenceFunc([](void* reader) { static_cast<TReader*>(reader)->PopSequence(); }),
			m_PushIndexFunc([](void* reader, size_t index) { static_cast<TReader*>(reader)->PushIndex(index); }),
			m_PopIndexFunc([](void* reader) { static_cast<TReader*>(reader)->PopIndex(); }),
			m_FindKeyFunc([](void* reader, const char* key, size_t& index) -> bool { return static_cast<TReader*>(reader)->FindKey(key, index); }),
			m_IsNullFunc([](void* reader) -> bool { return static_cast<TReader*>(reader)->IsNull(); }),
			m_SupportsBinary(reader.SupportsBinary())
		{

		}

		template<typename TReader>
		static Reader Create(const TReader& reader) { return Reader{ reader }; }

	public:
		READER_COMMON_BODY;

	private:
		void* m_Reader;

		void (*m_ReadBoolFunc)(void*, bool&);
		void (*m_ReadCharFunc)(void*, char&);
		void (*m_ReadWCharFunc)(void*, wchar_t&);
		void (*m_ReadInt8Func)(void*, int8_t&);
		void (*m_ReadInt16Func)(void*, int16_t&);
		void (*m_ReadInt32Func)(void*, int32_t&);
		void (*m_ReadInt64Func)(void*, int64_t&);
		void (*m_ReadUInt8Func)(void*, uint8_t&);
		void (*m_ReadUInt16Func)(void*, uint16_t&);
		void (*m_ReadUInt32Func)(void*, uint32_t&);
		void (*m_ReadUInt64Func)(void*, uint64_t&);
		void (*m_ReadFloatFunc)(void*, float&);
		void (*m_ReadDoubleFunc)(void*, double&);
		void (*m_ReadCStrFunc)(void*, char*);
		void (*m_ReadCWStrFunc)(void*, wchar_t*);
		void (*m_ReadStrFunc)(void*, std::string&);
		void (*m_ReadWStrFunc)(void*, std::wstring&);
		void (*m_ReadPathFunc)(void*, std::filesystem::path&);
		void (*m_ReadBinaryFunc)(void*, void*, size_t);

		bool(*m_IsMapFunc)(void*);
		size_t(*m_PushMapFunc)(void*);
		void (*m_PopMapFunc)(void*);
		void (*m_PushKeyFunc)(void*, size_t);
		void (*m_PopKeyFunc)(void*);
		void (*m_PushValueFunc)(void*, size_t);
		void (*m_PopValueFunc)(void*);
		size_t(*m_PushSequenceFunc)(void*);
		void (*m_PopSequenceFunc)(void*);
		void (*m_PushIndexFunc)(void*, size_t);
		void (*m_PopIndexFunc)(void*);
		bool (*m_FindKeyFunc)(void*, const char*, size_t&);
		bool (*m_IsNullFunc)(void*);

		bool m_SupportsBinary;

	};
}