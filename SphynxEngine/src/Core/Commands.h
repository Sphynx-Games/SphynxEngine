#pragma once

#include "Core/Core.h"
#include <string>

#pragma warning(push, 0)
#include <spdlog/formatter.h>
#pragma warning(pop)


namespace Sphynx
{
	enum class CommandArgument {
		DIRECTORY,
		MODULES,
		UNKNOWN
	};

	class CommandArgumentParser
	{
	public:
		static CommandArgument Parse(const std::string& arg)
		{
			if (s_ArgumentMap.ContainsKey(arg))
			{
				return s_ArgumentMap[arg];
			}
			return CommandArgument::UNKNOWN;
		}

	private:
		inline static HashMap<std::string, CommandArgument> s_ArgumentMap = {
			{"-d",          CommandArgument::DIRECTORY},
			{"--directory", CommandArgument::DIRECTORY},
			{"-m",          CommandArgument::MODULES},
			{"-modules",    CommandArgument::MODULES}
		};
	};
}