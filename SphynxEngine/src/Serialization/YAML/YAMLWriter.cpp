#include "spxpch.h"
#include "YAMLWriter.h"

#include <yaml-cpp/yaml.h>


namespace Sphynx
{
	YAMLWriter::YAMLWriter(const std::filesystem::path& path) :
		m_File(path, std::ios::out),
		m_Emitter(std::make_unique<YAML::Emitter>(m_File))
	{
	}

	YAMLWriter::~YAMLWriter()
	{
		if (m_File.is_open())
		{
			m_File.close();
		}
	}

	void YAMLWriter::Write(bool v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(double v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(float v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(uint64_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(uint32_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(uint16_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(uint8_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(int64_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(int32_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(int16_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(int8_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(wchar_t v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(char v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(const char* v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(const wchar_t* v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(const std::string& v)
	{
		*m_Emitter << v;
	}

	void YAMLWriter::Write(const std::wstring& v)
	{
		Write(v.c_str());
	}

	void YAMLWriter::Write(const std::filesystem::path& v)
	{
		Write(v.string());
	}

	void YAMLWriter::Write(const void* v, size_t size)
	{
		SPX_CORE_ASSERT(false, "Binary is not supported!");
	}

	void YAMLWriter::PushKey()
	{
		*m_Emitter << YAML::Key;
	}

	void YAMLWriter::PushValue()
	{
		*m_Emitter << YAML::Value;
	}

	void YAMLWriter::PushFlow()
	{
		*m_Emitter << YAML::Flow;
	}

	void YAMLWriter::PushSequence()
	{
		*m_Emitter << YAML::BeginSeq;
	}

	void YAMLWriter::PopSequence()
	{
		*m_Emitter << YAML::EndSeq;
	}

	void YAMLWriter::PushMap()
	{
		*m_Emitter << YAML::BeginMap;
	}

	void YAMLWriter::PopMap()
	{
		*m_Emitter << YAML::EndMap;
	}

	bool YAMLWriter::SupportsBinary() const
	{
		return false;
	}

}