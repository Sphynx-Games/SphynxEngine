#include <iostream>

#include <Sphynx.h>


class SandboxApplication : public Sphynx::Application
{
public:
	virtual void Init() override { Sphynx::Application::Init(); }
	virtual void Run() override { Sphynx::Application::Run(); }
	virtual void Shutdown() override { Sphynx::Application::Shutdown(); }
};

Sphynx::Application* CreateApplication()
{
	return new SandboxApplication();
}