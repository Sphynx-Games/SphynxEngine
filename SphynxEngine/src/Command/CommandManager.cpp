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
	

	void AddCoreCommandAlias()
	{
		for (int bit = 0; bit < NumCoreCommands; ++bit)
		{
			CoreCommand flag = static_cast<CoreCommand>(1 << bit);
			Array<std::string> commandNames = CommandParser::Parse(flag);
			for (int i = 1; i < commandNames.Size(); ++i)
			{
				CommandManager::AddAlias(commandNames[0], commandNames[i]);
			}
		}
	}

	void CommandManager::Init()
	{
		// TODO: generalize this Init function
		Array<std::string> directoryNames = CommandParser::Parse(CoreCommand::DIRECTORY);
		CommandWithArgsDelegate directoryFunction;
		directoryFunction.Bind([](const Array<std::string>& args) {
#ifdef SPX_PLATFORM_WINDOWS
			SetCurrentDirectory(args[0].c_str());
#endif
		});

		Array<std::string> modulesNames = CommandParser::Parse(CoreCommand::MODULES);
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

		CreateCommand(directoryNames[0], directoryFunction);
		CreateCommand(modulesNames[0], modulesFunction);
		AddCoreCommandAlias();
		/*AddAlias(directoryNames[0], directoryNames[1]);
		AddAlias(modulesNames[0], modulesNames[1]);*/
	}

	void CommandManager::Shutdown()
	{
		for (auto& [name, command] : s_Commands)
		{
			delete command;
		}
		s_Commands.RemoveAll();
		s_Alias.RemoveAll();
	}

	void CommandManager::CreateCommand(const std::string& name, const CommandNoArgsDelegate& function, CommandConfigFlags flags, const std::string& description)
	{
		const std::string* realName = GetCommandRealName(name);
		if (realName != nullptr)
		{
			SPX_CORE_LOG_WARNING("Overriding the behavoiur of the command '{}'", name.c_str());
		}
		else
		{
			realName = &name;
		}
		delete s_Commands[*realName];
		s_Commands[*realName] = new CommandNoArgs(*realName, description, flags, function);
	}

	void CommandManager::CreateCommand(const std::string& name, const CommandWithArgsDelegate& function, CommandConfigFlags flags, const std::string& description)
	{
		const std::string* realName = GetCommandRealName(name);
		if (realName != nullptr)
		{
			SPX_CORE_LOG_WARNING("Overriding the behavoiur of the command '{}'", name.c_str());
		}
		else
		{
			realName = &name;
		}
		delete s_Commands[*realName];
		s_Commands[*realName] = new CommandWithArgs(*realName, description, flags, function);
	}

	void Sphynx::CommandManager::ExecuteCommand(const std::string& name, const Array<std::string>& args)
	{
		const std::string* realName = GetCommandRealName(name);
		if (realName == nullptr)
		{
			SPX_CORE_LOG_WARNING("Command '{}' not found", name.c_str());
			return;
		}

		Command* command = s_Commands[*realName];

#if defined(SPX_DEBUG)
		if (command->ConfigFlags & CommandConfigFlags_Debug) {
			command->Execute(args);
		}
#elif defined(SPX_RELEASE)
		if (command->ConfigFlags & CommandConfigFlags_Release) {
			command->Execute(args);
		}
#elif defined(SPX_SHIPPING)
		if (command->ConfigFlags & CommandConfigFlags_Shipping) {
			command->Execute(args);
		}
#endif
	}

	void CommandManager::AddAlias(const std::string& name, const std::string& alias)
	{
		if (s_Alias.ContainsKey(alias))
		{
			SPX_CORE_LOG_WARNING("Alias '{}' already present", alias.c_str());
			return;
		}
		s_Alias[alias] = name;
	}

	const std::string* CommandManager::GetCommandRealName(const std::string& name)
	{
		if (s_Commands.ContainsKey(name))
		{
			return &name;
		}
		else if (s_Alias.ContainsKey(name))
		{
			return &s_Alias[name];
		}
		return nullptr;
	}
}