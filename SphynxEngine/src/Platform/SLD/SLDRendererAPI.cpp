#include "SLDRendererAPI.h"
#include "Core/Application.h"
#include "Renderer/Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "SDLUtils.h"

namespace Sphynx
{
	SDLRendererAPI::SDLRendererAPI() : m_Window(nullptr), m_Renderer(nullptr)
	{
		//Init();
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

	void SDLRendererAPI::SetViewport(glm::vec2 position, uint32_t width, uint32_t height)
	{
		SDL_Rect rect = { position.x, position.y, width, height };
		SDL_SetRenderViewport(m_Renderer, &rect);
	}

	void SDLRendererAPI::Clear(glm::vec4 color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.x, color.y, color.z, color.t);
		SDL_RenderClear(m_Renderer);
	}

	void SDLRendererAPI::Present()
	{
		SDL_RenderPresent(m_Renderer);
	}

	void SDLRendererAPI::DrawPoint(glm::vec2 point, glm::vec4 color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.x, color.y, color.z, color.t);

		ChangeToSphynxCoords(point, m_Window);
		SDL_RenderPoint(m_Renderer, point.x, point.y);
	}

	void SDLRendererAPI::DrawLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.x, color.y, color.z, color.t);

		ChangeToSphynxCoords(point1, m_Window);
		ChangeToSphynxCoords(point2, m_Window);
		SDL_RenderLine(m_Renderer, point1.x, point1.y, point2.x, point2.y);
	}

	void SDLRendererAPI::DrawQuad(glm::vec2 point, uint32_t width, uint32_t height, glm::vec4 color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.x, color.y, color.z, color.t);

		ChangeToSphynxCoords(point, m_Window);
		SDL_FRect rect = { point.x, point.y, width, height };
		SDL_RenderRect(m_Renderer, &rect);
	}

	void SDLRendererAPI::DrawTriangle(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3, glm::vec4 color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.x, color.y, color.z, color.t);

		ChangeToSphynxCoords(point1, m_Window);
		ChangeToSphynxCoords(point2, m_Window);
		ChangeToSphynxCoords(point3, m_Window);
		SDL_RenderLine(m_Renderer, point1.x, point1.y, point2.x, point2.y);
		SDL_RenderLine(m_Renderer, point1.x, point1.y, point3.x, point3.y);
		SDL_RenderLine(m_Renderer, point2.x, point2.y, point3.x, point3.y);
	}

	void SDLRendererAPI::DrawCircle(glm::vec2 point, float radius, uint32_t numSegments, glm::vec4 color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.x, color.y, color.z, color.t);

		float PI = 3.14;
		float circumference = radius * 2 * PI;
		float alpha = (2 * PI) / numSegments; // in radians

		for (int i = 0; i < numSegments; ++i)
		{
			float x0, y0, x1, y1;
			x0 = radius * cos(alpha * i) + point.x;
			y0 = radius * sin(alpha * i) + point.y;
			x1 = radius * cos(alpha * ((i + 1) % numSegments)) + point.x;
			y1 = radius * sin(alpha * ((i + 1) % numSegments)) + point.y;

			SDL_RenderLine(m_Renderer, x0, y0, x1, y1);
		}
	}

	/*void SDLRendererAPI::SetLineWidth(float width)
	{
	}*/
}