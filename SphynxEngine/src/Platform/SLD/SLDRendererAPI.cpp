#include "SLDRendererAPI.h"
#include "Core/Application.h"
#include "Logging/Log.h"
#include "Renderer/Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Renderer/Renderer2D.h"
#include "SDLUtils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

namespace Sphynx
{
	SDLRendererAPI::SDLRendererAPI() : m_Window(nullptr), m_Renderer(nullptr)
	{
		//Init();
	}

	SDLRendererAPI::~SDLRendererAPI()
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}

	void SDLRendererAPI::Init()
	{
		// Get window reference
		Application* app = Application::GetInstance();
		m_Window = app->GetWindow();
		if (m_Window == nullptr) {
			SPX_LOG_CORE_ERROR("Window not initialized!");
			return;
		}
		SDL_Window* window_SDL = reinterpret_cast<SDL_Window*>(m_Window->GetNativeWindow());

		// Create renderer
		m_Renderer = SDL_CreateRenderer(window_SDL, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (m_Renderer == nullptr) {
			SPX_LOG_CORE_ERROR("SDL renderer could not initialize! SDL_Error: {}", SDL_GetError());
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
		DrawTriangle(Renderer2D::GetRendererConfig().DrawMode, point1, point2, point3, color);
	}

	void SDLRendererAPI::DrawCircle(Vector2i point, float radius, uint32_t numSegments, Color color)
	{
		DrawCircle(Renderer2D::GetRendererConfig().DrawMode, point, radius, numSegments, color);
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
		ChangeToSphynxCoords(point1, m_Window);
		ChangeToSphynxCoords(point2, m_Window);
		ChangeToSphynxCoords(point3, m_Window);

		switch (drawMode) {
		case DrawMode::WIREFRAME:
		{
			SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

			const SDL_FPoint points[] =
			{
				{point1.X, point1.Y},
				{point2.X, point2.Y},
				{point3.X, point3.Y},
				{point1.X, point1.Y}
			};
			SDL_RenderLines(m_Renderer, points, sizeof(points) / sizeof(SDL_FPoint));
		}
		break;

		case DrawMode::FILLED:
		{
			const SDL_Vertex vertices[] =
			{
				{{point1.X, point1.Y}, {color.R, color.G, color.B, color.A}, {}},
				{{point2.X, point2.Y}, {color.R, color.G, color.B, color.A}, {}},
				{{point3.X, point3.Y}, {color.R, color.G, color.B, color.A}, {}}
			};
			const int32_t indices[] = { 0, 1, 2 };

			SDL_RenderGeometry(m_Renderer, nullptr, vertices, 3, indices, 3);
		}
		break;
		}
	}

	void SDLRendererAPI::DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color)
	{
		ChangeToSphynxCoords(center, m_Window);

		std::vector<SDL_FPoint> points;   // reserve space for "numSegments" number of points
		points.reserve(numSegments + 2);
		points.emplace_back(SDL_FPoint{ (float)center.X, (float)center.Y }); // push the center point

		std::vector<int32_t> indices;
		indices.reserve((numSegments + 1) * 3); // plus one to count the center

		// compute points and indices
		float PI = 3.14;
		float circumference = radius * 2 * PI;
		float alpha = (2 * PI) / numSegments; // in radians
		for (uint32_t i = 0; i < numSegments; ++i)
		{
			// push segments' first point
			float x = radius * std::cos(alpha * i) + center.X;
			float y = radius * std::sin(alpha * i) + center.Y;

			points.emplace_back(SDL_FPoint{ x, y });
			if (i == numSegments - 1) points.emplace_back(points[1]);

			// push segment indices
			indices.emplace_back(0);
			indices.emplace_back(i + 1);
			indices.emplace_back(((i + 1) % numSegments) + 1);
		}

		switch (drawMode) {
		case DrawMode::WIREFRAME:
		{
			SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);
			SDL_RenderLines(m_Renderer, points.data() + 1, points.size() - 1);
		}
		break;

		case DrawMode::FILLED:
		{
			SDL_Color sdlColor = { color.R, color.G, color.B, color.A };
			SDL_RenderGeometryRaw(
				m_Renderer,
				nullptr,
				(float*)points.data(), sizeof(float) * 2,
				&sdlColor, 0,
				nullptr, 0,
				points.size() - 1,
				indices.data(), indices.size(), sizeof(int32_t));
		}
		break;
		}
	}

	/*void SDLRendererAPI::SetLineWidth(float width)
	{
	}*/
}