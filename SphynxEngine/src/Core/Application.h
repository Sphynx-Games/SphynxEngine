#pragma once

#include "Core.h"

namespace Sphynx
{
	class SPHYNX_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Init();
		virtual void Run();
		virtual void Shutdown();

	};

	// Should be defined in client
	Application* CreateApplication();

}
