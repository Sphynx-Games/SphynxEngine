#include "spxpch.h"
#include "YAMLReader.h"

#include <yaml-cpp/yaml.h>


namespace Sphynx
{
	YAMLReader::YAMLReader(const std::filesystem::path& path) :
		m_File(path, std::ios::in),
		m_Node(std::make_shared<YAML::Node>(YAML::Load(m_File))),
		m_Stack()
	{
		m_Stack.push(m_Node);
	}

	YAMLReader::~YAMLReader()
	{
		if (m_File.is_open())
		{
			m_File.close();
		}
	}

	bool YAMLReader::IsValid() const
	{
		return m_File.is_open() && !m_Node->IsNull();
	}

	void YAMLReader::Read(bool& v) const
	{
		v = m_Node->as<bool>();
	}

	void YAMLReader::Read(char& v) const
	{
		v = m_Node->as<char>();
	}

	void YAMLReader::Read(wchar_t& v) const
	{
		SPX_CORE_ASSERT(false, "wchat_t is not supported!");
		//v = m_Node->as<wchar_t>();
	}

	void YAMLReader::Read(int8_t& v) const
	{
		v = m_Node->as<int8_t>();
	}

	void YAMLReader::Read(int16_t& v) const
	{
		v = m_Node->as<int16_t>();
	}

	void YAMLReader::Read(int32_t& v) const
	{
		v = m_Node->as<int32_t>();
	}

	void YAMLReader::Read(int64_t& v) const
	{
		v = m_Node->as<int64_t>();
	}

	void YAMLReader::Read(uint8_t& v) const
	{
		v = m_Node->as<uint8_t>();
	}

	void YAMLReader::Read(uint16_t& v) const
	{
		v = m_Node->as<uint16_t>();
	}

	void YAMLReader::Read(uint32_t& v) const
	{
		v = m_Node->as<uint32_t>();
	}

	void YAMLReader::Read(uint64_t& v) const
	{
		v = m_Node->as<uint64_t>();
	}

	void YAMLReader::Read(float& v) const
	{
		v = m_Node->as<float>();
	}

	void YAMLReader::Read(double& v) const
	{
		v = m_Node->as<double>();
	}

	void YAMLReader::Read(char* v) const
	{
		SPX_CORE_ASSERT(false, "c-style string is not supported!");
		//v = m_Node->as<char*>();
	}

	void YAMLReader::Read(wchar_t* v) const
	{
		SPX_CORE_ASSERT(false, "c-style wstring is not supported!");
		//v = m_Node->as<wchar_t*>();
	}

	void YAMLReader::Read(std::string& v) const
	{
		v = m_Node->as<std::string>();
	}

	void YAMLReader::Read(std::wstring& v) const
	{
		SPX_CORE_ASSERT(false, "wstring is not supported!");
		//v = m_Node->as<std::wstring>();
	}

	void YAMLReader::Read(std::filesystem::path& v) const
	{
		std::string s; Read(s);
		v = s;
	}

	void YAMLReader::Read(void* v, size_t size) const
	{
		SPX_CORE_ASSERT(false, "Binary is not supported!");
	}

	void YAMLReader::PushIndex(size_t index) const
	{
		auto it = m_Node->begin(); std::advance(it, index);
		SPX_CORE_ASSERT(it != m_Node->end(), "Node iterator is out of bounds!");

		m_Stack.push(std::make_shared<YAML::Node>(*it));
		m_Node = m_Stack.top();
	}

	void YAMLReader::PopIndex() const
	{
		Pop();
	}

	void YAMLReader::PushKey(size_t index) const
	{
		auto it = m_Node->begin(); std::advance(it, index);
		SPX_CORE_ASSERT(it != m_Node->end(), "Node iterator is out of bounds!");

		m_Stack.push(std::make_shared<YAML::Node>(it->first));
		m_Node = m_Stack.top();
	}

	void YAMLReader::PopKey() const
	{
		Pop();
	}

	void YAMLReader::PushValue(size_t index) const
	{
		auto it = m_Node->begin(); std::advance(it, index);
		SPX_CORE_ASSERT(it != m_Node->end(), "Node iterator is out of bounds!");

		m_Stack.push(std::make_shared<YAML::Node>(it->second));
		m_Node = m_Stack.top();
	}

	void YAMLReader::PopValue() const
	{
		Pop();
	}

	size_t YAMLReader::PushSequence() const
	{
		SPX_CORE_ASSERT(m_Node->IsSequence(), "Current node is not a sequence!");
		return m_Node->size();
	}

	void YAMLReader::PopSequence() const
	{
		SPX_CORE_ASSERT(m_Node->IsSequence(), "Current node is not a sequence!");
	}

	bool YAMLReader::IsMap() const
	{
		return m_Node->IsMap();
	}

	size_t YAMLReader::PushMap() const
	{
		SPX_CORE_ASSERT(m_Node->IsMap(), "Current node is not a map!");
		return m_Node->size();
	}

	void YAMLReader::PopMap() const
	{
		SPX_CORE_ASSERT(m_Node->IsMap(), "Current node is not a map!");
	}

	void YAMLReader::Pop() const
	{
		m_Stack.pop();
		m_Node = m_Stack.top();

		SPX_CORE_ASSERT(m_Stack.size() >= 1, "Node stack should have at least one element!");
	}

	bool YAMLReader::FindKey(const char* key, size_t& index) const
	{
		auto it = std::find_if(m_Node->begin(), m_Node->end(), [&](const auto& node) 
			{
				std::string str = node.first.as<std::string>();
				return !strcmp(key, str.c_str());
			});

		if (it != m_Node->end())
		{
			index = std::distance(m_Node->begin(), it);
			return true;
		}

		return false;
	}

	bool YAMLReader::IsNull() const
	{
		return m_Node->IsNull();
	}

	bool YAMLReader::SupportsBinary() const
	{
		return false;
	}

}
