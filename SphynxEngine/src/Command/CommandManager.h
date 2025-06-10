#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Container/Map.h"
#include <string>


namespace Sphynx
{
	typedef Delegate<void()> CommandNoArgsDelegate;
	typedef Delegate<void(const Array<std::string>&)> CommandWithArgsDelegate;

	typedef int CommandConfigFlags; // enum CommandConfigFlags_

	enum CommandConfigFlags_
	{
		CommandConfigFlags_Debug = 0,
		CommandConfigFlags_Release = 1 << 0,
		CommandConfigFlags_Shipping = 1 << 1,
		CommandConfigFlags_All = CommandConfigFlags_Debug | CommandConfigFlags_Release | CommandConfigFlags_Shipping
	};

	class SPHYNX_API CommandManager
	{
	public:
		static void Init();
		static void Shutdown();

		static void CreateCommand(const std::string& name, const CommandNoArgsDelegate& function, CommandConfigFlags flags = CommandConfigFlags_All, const std::string& description = "");
		static void CreateCommand(const std::string& name, const CommandWithArgsDelegate& function, CommandConfigFlags flags = CommandConfigFlags_All, const std::string& description = "");
		
		static void ExecuteCommand(const std::string& name, const Array<std::string>& args);

		static void AddAlias(const std::string& name, const std::string& alias);

	private:
		static const std::string* GetCommandRealName(const std::string& name);

	private:
		inline static HashMap<std::string, struct Command*> s_Commands = {};
		inline static HashMap<std::string, std::string> s_Alias = {};
	};
}