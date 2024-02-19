#pragma once


#ifdef SPX_PLATFORM_WINDOWS

extern Sphynx::Application* CreateApplication();

int main(int argc, char** argv)
{
	Sphynx::Application* application = CreateApplication();
	
	application->Init();
	application->Run();
	application->Shutdown();

	delete application;

	return 0;
}

#endif