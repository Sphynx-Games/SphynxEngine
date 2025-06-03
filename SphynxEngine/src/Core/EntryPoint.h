#pragma once

#include "Core/Engine.h"
#include "Core/Commands.h"
#include "Container/Map.h"
#include <string.h>


#ifdef SPX_PLATFORM_WINDOWS

extern Sphynx::Application* CreateApplication();

int main(int argc, char** argv)
{
	// save options
	Sphynx::HashMap<Sphynx::CommandArgument, Sphynx::Array<std::string>> options;
	for (int i = 1; i < argc; ++i)
	{
		Sphynx::CommandArgument arg = Sphynx::CommandArgumentParser::Parse(argv[i]);
		switch (arg)
		{
		case Sphynx::CommandArgument::DIRECTORY:
		{
			if (i + 1 >= argc)
			{
				SPX_CORE_LOG_ERROR("Error: option '--directory' requires a value");
				return 1;
			}

			Sphynx::Array<std::string> values;
			values.Add(argv[i + 1]);
			options.Emplace(std::move(arg), std::move(values));
			++i;

			break;
		}

		case Sphynx::CommandArgument::MODULES:
		{
			Sphynx::Array<std::string> values;
			while (i + 1 < argc && argv[i + 1][0] != '-')
			{
				values.Add(argv[i + 1]);
				++i;
			}
			options.Emplace(std::move(arg), std::move(values));

			break;
		}

		default:
		{
			SPX_CORE_LOG_WARNING("Option '{}' not recognised", argv[i]);
			break;
		}
		}
	}

	// start application
	Sphynx::Engine::Init();

	Sphynx::Application* application = CreateApplication();

	application->Init(options);
	application->Run();
	application->Shutdown();

	delete application;

	Sphynx::Engine::Shutdown();

	return 0;
}

#endif