#include "SLDRendererAPI.h"
#include "Core/Application.h"
#include "Renderer/Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Renderer/Renderer2D.h"
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

	void SDLRendererAPI::SetViewport(Vector2i position, uint32_t width, uint32_t height)
	{
		SDL_Rect rect = { position.X, position.Y, width, height };
		SDL_SetRenderViewport(m_Renderer, &rect);
	}

	void SDLRendererAPI::Clear(Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);
		SDL_RenderClear(m_Renderer);
	}

	void SDLRendererAPI::Present()
	{
		SDL_RenderPresent(m_Renderer);
	}

	void SDLRendererAPI::DrawPoint(Vector2i point, Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		ChangeToSphynxCoords(point, m_Window);
		SDL_RenderPoint(m_Renderer, point.X, point.Y);
	}

	void SDLRendererAPI::DrawLine(Vector2i point1, Vector2i point2, Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		ChangeToSphynxCoords(point1, m_Window);
		ChangeToSphynxCoords(point2, m_Window);
		SDL_RenderLine(m_Renderer, point1.X, point1.Y, point2.X, point2.Y);
	}

	void SDLRendererAPI::DrawQuad(Vector2i point, uint32_t width, uint32_t height, Color color)
	{
		DrawQuad(Renderer2D::GetRendererConfig().DrawMode, point, width, height, color);
	}

	void SDLRendererAPI::DrawTriangle(Vector2i point1, Vector2i point2, Vector2i point3, Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		ChangeToSphynxCoords(point1, m_Window);
		ChangeToSphynxCoords(point2, m_Window);
		ChangeToSphynxCoords(point3, m_Window);
		SDL_RenderLine(m_Renderer, point1.X, point1.Y, point2.X, point2.Y);
		SDL_RenderLine(m_Renderer, point1.X, point1.Y, point3.X, point3.Y);
		SDL_RenderLine(m_Renderer, point2.X, point2.Y, point3.X, point3.Y);
	}

	void SDLRendererAPI::DrawCircle(Vector2i point, float radius, uint32_t numSegments, Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		float PI = 3.14;
		float circumference = radius * 2 * PI;
		float alpha = (2 * PI) / numSegments; // in radians

		for (int i = 0; i < numSegments; ++i)
		{
			float x0, y0, x1, y1;
			x0 = radius * cos(alpha * i) + point.X;
			y0 = radius * sin(alpha * i) + point.Y;
			x1 = radius * cos(alpha * ((i + 1) % numSegments)) + point.X;
			y1 = radius * sin(alpha * ((i + 1) % numSegments)) + point.Y;

			SDL_RenderLine(m_Renderer, x0, y0, x1, y1);
		}
	}

	void SDLRendererAPI::DrawQuad(DrawMode drawMode, Vector2i point, uint32_t width, uint32_t height, Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		ChangeToSphynxCoords(point, m_Window);
		SDL_FRect rect = { point.X, point.Y, width, height };

		switch (drawMode) {
		case DrawMode::WIREFRAME:
			SDL_RenderRect(m_Renderer, &rect);
			break;
		case DrawMode::FILLED:
			SDL_RenderFillRect(m_Renderer, &rect);
			break;
		}
	}

	void SDLRendererAPI::DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color)
	{
		
	}

	void SDLRendererAPI::DrawCircle(DrawMode drawMode, Vector2i point, float radius, uint32_t numSegments, Color color)
	{
		
	}

	/*void SDLRendererAPI::SetLineWidth(float width)
	{
	}*/
}