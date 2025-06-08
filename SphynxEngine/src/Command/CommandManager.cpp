#include "CommandManager.h"
#include "CoreCommands.h"
#include "Module/ModuleManager.h"
#include "Asset/AssetManager.h"

#include <sstream>


namespace Sphynx
{
	struct Command
	{
	public:
		Command(const std::string& name, std::string description, CommandConfigFlags configFlags) :
			Name(name),
			Description(description),
			ConfigFlags(configFlags)
		{}

		virtual void Execute(const Array<std::string>& args) = 0;

	public:
		std::string Name;
		std::string Description;
		CommandConfigFlags ConfigFlags;
	};

	struct CommandNoArgs : public Command
	{
	public:
		CommandNoArgs(const std::string& name, std::string description, CommandConfigFlags configFlags, CommandNoArgsDelegate function) :
			Command(name, description, configFlags),
			Function(function)
		{}

		virtual void Execute(const Array<std::string>& args) override
		{
			Function.Execute();
		}

	private:
		CommandNoArgsDelegate Function;
	};

	struct CommandWithArgs : public Command
	{
	public:
		CommandWithArgs(const std::string& name, std::string description, CommandConfigFlags configFlags, CommandWithArgsDelegate function) :
			Command(name, description, configFlags),
			Function(function)
		{}

		virtual void Execute(const Array<std::string>& args) override
		{
			Function.Execute(args);
		}

	private:
		CommandWithArgsDelegate Function;
	};
	

	void CommandManager::Init()
	{
		const std::string directoryName = CommandParser::Parse(CoreCommand::DIRECTORY)[0];
		CommandWithArgsDelegate directoryFunction;
		directoryFunction.Bind([](const Array<std::string>& args) {
#ifdef SPX_PLATFORM_WINDOWS
			SetCurrentDirectory(args[0].c_str());
#endif
		});

		const std::string modulesName = CommandParser::Parse(CoreCommand::MODULES)[0];
		CommandWithArgsDelegate modulesFunction;
		modulesFunction.Bind([](const Array<std::string>& args) {
			for (const std::string& moduleName : args)
			{
				ModuleManager::LoadModule(MODULE_PATH(moduleName));
			}

			// TODO: delete this lines in the future
			AssetManager::Shutdown();
			Reflection::Registry::Shutdown();
			Reflection::Registry::Init();
			AssetManager::Init();
		});

		CreateCommand(directoryName, directoryFunction);
		CreateCommand(modulesName, modulesFunction);
	}

	void CommandManager::Shutdown()
	{
		for (auto& [name, command] : s_Commands)
		{
			delete command;
		}
		s_Commands.RemoveAll();
	}

	void CommandManager::CreateCommand(const std::string& name, const CommandNoArgsDelegate& function, CommandConfigFlags flags, const std::string& description)
	{
		if (s_Commands.ContainsKey(name))
		{
			SPX_CORE_LOG_WARNING("Overriding the behavoiur of the command '{}'", name);
		}
		s_Commands[name] = new CommandNoArgs(name, description, flags, function);
	}

	void CommandManager::CreateCommand(const std::string& name, const CommandWithArgsDelegate& function, CommandConfigFlags flags, const std::string& description)
	{
		if (s_Commands.ContainsKey(name))
		{
			SPX_CORE_LOG_WARNING("Overriding the behavoiur of the command '{}'", name.c_str());
		}
		s_Commands[name] = new CommandWithArgs(name, description, flags, function);
	}

	void Sphynx::CommandManager::ExecuteCommand(const std::string& name, const Array<std::string>& args)
	{
		if (!s_Commands.ContainsKey(name))
		{
			SPX_CORE_LOG_WARNING("Command '{}' not found", name.c_str());
			return;
		}
		s_Commands[name]->Execute(args);
	}
}