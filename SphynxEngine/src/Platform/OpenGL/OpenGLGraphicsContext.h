#pragma once

#include "Core/Core.h"
#include "Renderer/GraphicsContext.h"
#include "SDL3/SDL_video.h"


namespace Sphynx
{
	class SPHYNX_API OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		OpenGLGraphicsContext(Window* window);
		virtual ~OpenGLGraphicsContext();

		virtual void Init() override;
		void Shutdown() override;

		virtual void OnBeforeWindowCreation() override;

		virtual void SwapBuffers() override;

	private:
		SDL_GLContext m_Context;

	};
}
