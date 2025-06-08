#pragma once

#include "Core/Engine.h"
#include "Container/Map.h"
#include <string.h>


#ifdef SPX_PLATFORM_WINDOWS

extern Sphynx::Application* CreateApplication();

int main(int argc, char** argv)
{
	// save command line argument options
	Sphynx::HashMap<std::string, Sphynx::Array<std::string>> commandArguments;
	for (int i = 1; i < argc; ++i)
	{
		std::string key = argv[i];
		Sphynx::Array<std::string> values;
		while (i + 1 < argc && argv[i + 1][0] != '-')
		{
			values.Add(argv[i + 1]);
			++i;
		}
		commandArguments.Emplace(std::move(key), std::move(values));
	}

	// start application
	Sphynx::Engine::Init();

	Sphynx::Application* application = CreateApplication();

	application->Init(commandArguments);
	application->Run();
	application->Shutdown();

	delete application;

	Sphynx::Engine::Shutdown();

	return 0;
}

#endif