#pragma once

#include "Core/Engine.h"


#ifdef SPX_PLATFORM_WINDOWS

extern Sphynx::Application* CreateApplication();

int main(int argc, char** argv)
{
	Sphynx::Engine::Init();

	Sphynx::Application* application = CreateApplication();
	
	application->Init();
	application->Run();
	application->Shutdown();

	delete application;

	Sphynx::Engine::Shutdown();

	return 0;
}

#endif