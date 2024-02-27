#include "SLDRendererAPI.h"
#include "Core/Application.h"
#include "Logging/Log.h"
#include "Renderer/Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Renderer/Renderer2D.h"
#include "SDLUtils.h"
#include <SDL3/SDL.h>
#include <glm/ext/matrix_transform.hpp>

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

	void SDLRendererAPI::SetViewport(Vector2i position, Vector2i size)
	{
		SDL_Rect rect = { position.X, position.Y, size.X, size.Y };
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

	void SDLRendererAPI::DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size, Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		ChangeToSphynxCoords(center, m_Window);
		SDL_FRect rect = { center.X, center.Y, size.X, size.Y };

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

		std::vector<SDL_FPoint> points;
		points.reserve(4);
		points.emplace_back(SDL_FPoint{ (float)point1.X, (float)point1.Y });
		points.emplace_back(SDL_FPoint{ (float)point2.X, (float)point2.Y });
		points.emplace_back(SDL_FPoint{ (float)point3.X, (float)point3.Y });
		points.emplace_back(SDL_FPoint{ (float)point1.X, (float)point1.Y });

		std::vector<int32_t> indices({ 0, 1, 2 });

		DrawPoligon(m_Renderer, drawMode, points, indices, color);
	}

	void SDLRendererAPI::DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color)
	{
		ChangeToSphynxCoords(center, m_Window);

		std::vector<SDL_FPoint> points; // reserve space for "numSegments" number of points
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

		DrawPoligon(m_Renderer, drawMode, points, indices, color, true);
	}

	void SDLRendererAPI::DrawQuad(DrawMode drawMode, const Transform& transform, Vector2i size, Vector2f pivot, Color color)
	{
		// multiply transformation matrices
		glm::mat4 mat = MultTransformMatrices(transform);

		// calculate rectangle corners with transformations applied
		float midWidth = (size.X / 2.0f);
		float midHeight = (size.Y / 2.0f);

		Vector2i ul = { - midWidth, + midHeight };
		Vector2i ur = { + midWidth, + midHeight };
		Vector2i dr = { + midWidth, - midHeight };
		Vector2i dl = { - midWidth, - midHeight };
		ChangeToSphynxCoords(ul, m_Window);
		ChangeToSphynxCoords(ur, m_Window);
		ChangeToSphynxCoords(dr, m_Window);
		ChangeToSphynxCoords(dl, m_Window);

		glm::vec4 UL = mat * glm::vec4{ ul.X, ul.Y, 0.0f, 1.0f };
		glm::vec4 UR = mat * glm::vec4{ ur.X, ur.Y, 0.0f, 1.0f };
		glm::vec4 DR = mat * glm::vec4{ dr.X, dr.Y, 0.0f, 1.0f };
		glm::vec4 DL = mat * glm::vec4{ dl.X, dl.Y, 0.0f, 1.0f };

		// define points and indices to draw
		std::vector<SDL_FPoint> points;
		points.reserve(5);
		points.emplace_back(SDL_FPoint{ (float)UL.x, (float)UL.y });
		points.emplace_back(SDL_FPoint{ (float)UR.x, (float)UR.y });
		points.emplace_back(SDL_FPoint{ (float)DR.x, (float)DR.y });
		points.emplace_back(SDL_FPoint{ (float)DL.x, (float)DL.y });
		points.emplace_back(SDL_FPoint{ (float)UL.x, (float)UL.y });

		std::vector<int32_t> indices({ 0, 1, 2, 2, 3, 0 });

		DrawPoligon(m_Renderer, drawMode, points, indices, color);
	}

	void SDLRendererAPI::DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2i point1, Vector2i point2, Vector2i point3, Vector2f pivot, Color color)
	{
		ChangeToSphynxCoords(point1, m_Window);
		ChangeToSphynxCoords(point2, m_Window);
		ChangeToSphynxCoords(point3, m_Window);

		glm::mat4 mat = MultTransformMatrices(transform);

		glm::vec4 P1 = mat * glm::vec4{ point1.X, point1.Y, 0.0f, 1.0f };
		glm::vec4 P2 = mat * glm::vec4{ point2.X, point2.Y, 0.0f, 1.0f };
		glm::vec4 P3 = mat * glm::vec4{ point3.X, point3.Y, 0.0f, 1.0f };

		std::vector<SDL_FPoint> points;
		points.reserve(4);
		points.emplace_back(SDL_FPoint{ (float)P1.x, (float)P1.y });
		points.emplace_back(SDL_FPoint{ (float)P2.x, (float)P2.y });
		points.emplace_back(SDL_FPoint{ (float)P3.x, (float)P3.y });
		points.emplace_back(SDL_FPoint{ (float)P1.x, (float)P1.y });

		std::vector<int32_t> indices({ 0, 1, 2 });

		DrawPoligon(m_Renderer, drawMode, points, indices, color);
	}

	void SDLRendererAPI::DrawCircle(DrawMode drawMode, const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot, Color color)
	{
		//ChangeToSphynxCoords(center, m_Window);

		glm::mat4 mat = MultTransformMatrices(transform);

		std::vector<SDL_FPoint> points; // reserve space for "numSegments" number of points
		points.reserve(numSegments + 2);
		points.emplace_back(SDL_FPoint{ (float)transform.Position.X, (float)transform.Position.Y }); // push the center point

		std::vector<int32_t> indices;
		indices.reserve((numSegments + 1) * 3); // plus one to count the center

		// compute points and indices
		float PI = 3.14;
		float circumference = radius * 2 * PI;
		float alpha = (2 * PI) / numSegments; // in radians
		for (uint32_t i = 0; i < numSegments; ++i)
		{
			// push segments' first point
			float x = radius * std::cos(alpha * i) + transform.Position.X;
			float y = radius * std::sin(alpha * i) + transform.Position.Y;

			glm::vec4 P = mat * glm::vec4{ x, y, 0.0f, 1.0f };
			points.emplace_back(SDL_FPoint{ P.x, P.y });
			if (i == numSegments - 1) points.emplace_back(points[1]);

			// push segment indices
			indices.emplace_back(0);
			indices.emplace_back(i + 1);
			indices.emplace_back(((i + 1) % numSegments) + 1);
		}

		DrawPoligon(m_Renderer, drawMode, points, indices, color, true);
	}

	/*void SDLRendererAPI::SetLineWidth(float width)
	{
	}*/
}