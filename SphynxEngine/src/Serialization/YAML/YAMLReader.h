#pragma once

#include "Core/Core.h"
#include "Serialization/Reader.h"

#include <fstream>
#include <iostream>
#include <stack>


namespace YAML { class Node; }

namespace Sphynx
{
	class SPHYNX_API YAMLReader
	{
	public:
		YAMLReader(const std::filesystem::path& path);
		~YAMLReader();

	public:
		READER_COMMON_BODY;
		bool IsValid() const;

	private:
		void Pop() const;

	protected:
		std::ifstream m_File;
		mutable std::shared_ptr<YAML::Node> m_Node;
		mutable std::stack<std::shared_ptr<YAML::Node>> m_Stack;

	};
}