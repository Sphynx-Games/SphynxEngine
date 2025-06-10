#pragma once

#include "Core/Core.h"
#include <string>


namespace Sphynx
{
	enum class CoreCommand {
		DIRECTORY = 1 << 0,
		MODULES = 1 << 1,
		UNKNOWN = 0
	};

	int NumCoreCommands = 2;

	class CommandParser
	{
	public:
		static CoreCommand Parse(const std::string& arg)
		{
			if (s_CommandNameMap.ContainsKey(arg))
			{
				return s_CommandNameMap[arg];
			}
			return CoreCommand::UNKNOWN;
		}

		static const Array<std::string> Parse(CoreCommand arg)
		{
			Array<std::string> result;
			for (auto& [key, value] : s_CommandNameMap)
			{
				if (arg == value)
				{
					result.Add(key);
				}
			}
			return result;
		}

	private:
		inline static HashMap<std::string, CoreCommand> s_CommandNameMap = {
			{"-d",          CoreCommand::DIRECTORY},
			{"--directory", CoreCommand::DIRECTORY},
			{"-m",          CoreCommand::MODULES},
			{"--modules",   CoreCommand::MODULES}
		};
	};
}