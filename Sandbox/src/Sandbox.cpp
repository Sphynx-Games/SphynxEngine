#include <iostream>

#include <Sphynx.h>


class SandboxApplication : public Sphynx::Application
{

public:
	virtual void Init() override { printf("Application Initialized\n"); }
	virtual void Run() override { printf("Application Run\n"); }
	virtual void Shutdown() override { printf("Application Shutdown\n"); }

};

Sphynx::Application* CreateApplication()
{
	return new SandboxApplication();
}
