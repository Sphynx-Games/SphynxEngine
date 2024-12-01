#pragma once

#include "Core/Core.h"
#include "Renderer/GraphicsContext.h"


struct SDL_Renderer;

namespace Sphynx
{
	class SPHYNX_API SDLGraphicsContext : public GraphicsContext
	{
	public:
		SDLGraphicsContext(Window* window);
		virtual ~SDLGraphicsContext();

		virtual void Init() override;
		void Shutdown() override;

		virtual void OnBeforeWindowCreation() override;

		virtual void SwapBuffers() override;

	public:
		SDL_Renderer* GetRenderer() const;

	private:
		SDL_Renderer* m_Renderer;

	};
}
