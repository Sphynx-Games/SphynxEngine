#include "spxpch.h"
#include "SDLGraphicsContext.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "Renderer/Window.h"


namespace Sphynx
{
	SDLGraphicsContext::SDLGraphicsContext(Window* window) :
		GraphicsContext(window),
		m_Renderer(nullptr)
	{
	}

	SDLGraphicsContext::~SDLGraphicsContext()
	{
	}

	void SDLGraphicsContext::Init()
	{
		SDL_Window* window = reinterpret_cast<SDL_Window*>(m_Window->GetNativeWindow());

		SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "3");
		m_Renderer = SDL_CreateRenderer(window, nullptr);
		SPX_CORE_ASSERT(m_Renderer != nullptr, "Failed to create SDL renderer! Error: {}", SDL_GetError());
	}

	void SDLGraphicsContext::Shutdown()
	{
		SPX_CORE_ASSERT(m_Renderer != nullptr, "Renderer should be valid!");

		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}

	void SDLGraphicsContext::OnBeforeWindowCreation()
	{

	}

	void SDLGraphicsContext::SwapBuffers()
	{
		SDL_RenderPresent(m_Renderer);
	}

	SDL_Renderer* SDLGraphicsContext::GetRenderer() const
	{
		return m_Renderer;
	}

}
