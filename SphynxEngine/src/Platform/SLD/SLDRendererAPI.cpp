#include "SLDRendererAPI.h"
#include "Core/Application.h"
#include "Renderer/Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "SDLUtils.h"

namespace Sphynx
{
	SDLRendererAPI::SDLRendererAPI() : m_Window(nullptr), m_Renderer(nullptr)
	{
		Init();
	}

	SDLRendererAPI::~SDLRendererAPI()
	{
		SDL_DestroyRenderer(m_Renderer);
	}

	void SDLRendererAPI::Init()
	{
		// Get window reference
		Application* app = Application::GetInstance();
		m_Window = app->GetWindow();
		if (m_Window == nullptr) {
			printf("Window not initialized!\n"); 
			return;
		}
		SDL_Window* window_SDL = reinterpret_cast<SDL_Window*>(m_Window->GetNativeWindow());

		// Create renderer
		m_Renderer = SDL_CreateRenderer(window_SDL, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (m_Renderer == nullptr) {
			printf("SDL renderer could not initialize! SDL_Error: %s\n", SDL_GetError());
		}
	}

	void SDLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		SDL_Rect rect = { x, y, width, height };
		SDL_SetRenderViewport(m_Renderer, &rect);
	}

	void SDLRendererAPI::Clear(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
	{
		SDL_SetRenderDrawColor(m_Renderer, R, G, B, A);
		SDL_RenderClear(m_Renderer);
	}

	void SDLRendererAPI::Present()
	{
		SDL_RenderPresent(m_Renderer);
	}

	void SDLRendererAPI::DrawPoint(uint32_t x, uint32_t y, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
	{
		SDL_SetRenderDrawColor(m_Renderer, R, G, B, A);

		ChangeToSphynxCoords(x, y, m_Window);
		SDL_RenderPoint(m_Renderer, x, y);
	}

	void SDLRendererAPI::DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
	{
		SDL_SetRenderDrawColor(m_Renderer, R, G, B, A);

		ChangeToSphynxCoords(x1, y1, m_Window);
		ChangeToSphynxCoords(x2, y2, m_Window);
		SDL_RenderLine(m_Renderer, x1, y1, x2, y2);
	}

	void SDLRendererAPI::DrawQuad(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
	{
		SDL_SetRenderDrawColor(m_Renderer, R, G, B, A);

		ChangeToSphynxCoords(x, y, m_Window);
		SDL_FRect rect = { x, y, width, height };
		SDL_RenderRect(m_Renderer, &rect);
	}

	void SDLRendererAPI::DrawTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
	{
		SDL_SetRenderDrawColor(m_Renderer, R, G, B, A);

		ChangeToSphynxCoords(x1, y1, m_Window);
		ChangeToSphynxCoords(x2, y2, m_Window);
		ChangeToSphynxCoords(x3, y3, m_Window);
		SDL_RenderLine(m_Renderer, x1, y1, x2, y2);
		SDL_RenderLine(m_Renderer, x1, y1, x3, y3);
		SDL_RenderLine(m_Renderer, x2, y2, x3, y3);
	}

	void SDLRendererAPI::DrawCircle(uint32_t x, uint32_t y, float radius, uint32_t numSegments, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
	{
		SDL_SetRenderDrawColor(m_Renderer, R, G, B, A);

		float PI = 3.14;
		float circumference = radius * 2 * PI;
		float alpha = (2 * PI) / numSegments; // in radians

		for (int i = 0; i < numSegments; ++i)
		{
			float x0, y0, x1, y1;
			x0 = radius * cos(alpha * i) + x;
			y0 = radius * sin(alpha * i) + y;
			x1 = radius * cos(alpha * ((i + 1) % numSegments)) + x;
			y1 = radius * sin(alpha * ((i + 1) % numSegments)) + y;

			SDL_RenderLine(m_Renderer, x0, y0, x1, y1);
		}
	}

	/*void SDLRendererAPI::SetLineWidth(float width)
	{
	}*/
}