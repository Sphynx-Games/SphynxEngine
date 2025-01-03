#include "spxpch.h"
#include "SDLRendererAPI.h"
#include "Core/Application.h"
#include "Logging/Log.h"
#include "Renderer/Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/SDL/SDLTexture.h"
#include "Platform/SDL/SDLFont.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture.h"
#include "Renderer/Sprite.h"
#include "Renderer/Font.h"
#include "Math/Math.h"
#include "Math/Transform.h"
#include "Math/Matrix.h"
#include "SDLUtils.h"
#include <SDL3/SDL.h>
#include <glm/ext/matrix_transform.hpp>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_iostream.h>
#include "SDLGraphicsContext.h"


namespace Sphynx
{
	SDLRendererAPI::SDLRendererAPI() :
		m_Window(nullptr),
		m_Renderer(nullptr)
	{
	}

	SDLRendererAPI::~SDLRendererAPI()
	{
	}

	void SDLRendererAPI::Init()
	{
		// Get window reference
		m_Window = Application::GetInstance()->GetWindow();
		SPX_CORE_ASSERT(m_Window, "Window not initialized!");
		
		// Get renderer reference
		m_Renderer = static_cast<SDLGraphicsContext*>(m_Window->GetGraphicsContext())->GetRenderer();
		SPX_CORE_ASSERT(m_Renderer != nullptr, "Renderer is not valid!");
	}

	void SDLRendererAPI::Shutdown()
	{
		m_Renderer = nullptr;
		m_Window = nullptr;
	}

	void SDLRendererAPI::SetViewport(Vector2i position, Vector2i size)
	{
		SDL_Rect rect = { position.X, position.Y, size.X, size.Y };
		SDL_SetRenderViewport(m_Renderer, &rect);
	}

	Vector2i SDLRendererAPI::GetViewportSize()
	{
		SDL_Rect rect;
		SDL_GetRenderViewport(m_Renderer, &rect);
		return { (int32_t)rect.w, (int32_t)rect.h };
	}

	void SDLRendererAPI::Clear(Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);
		SDL_RenderClear(m_Renderer);
	}

	void SDLRendererAPI::Present()
	{
		SDL_Texture* renderTarget = SDL_GetRenderTarget(m_Renderer);
		if (renderTarget == nullptr)
		{
			// TODO: this feels counter intuitive as it also swap buffers
			//SDL_RenderPresent(m_Renderer);
			return;
		}

		SDL_RenderTexture(m_Renderer, renderTarget, nullptr, nullptr);
	}

	void SDLRendererAPI::DrawPoint(Vector2i point, Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		ChangeToSphynxCoords(point, m_Window);
		SDL_RenderPoint(m_Renderer, (float)point.X, (float)point.Y);
	}

	void SDLRendererAPI::DrawLine(Vector2i point1, Vector2i point2, float lineWidth, Color color)
	{
		if (lineWidth <= 0.0f) return;

		Vector2f scale{};
		SDL_GetRenderScale(m_Renderer, &scale.X, &scale.Y);
		SDL_SetRenderScale(m_Renderer, lineWidth, lineWidth);

		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		ChangeToSphynxCoords(point1, m_Window);
		ChangeToSphynxCoords(point2, m_Window);
		SDL_RenderLine(m_Renderer,
			(float)point1.X / lineWidth, (float)point1.Y / lineWidth,
			(float)point2.X / lineWidth, (float)point2.Y / lineWidth
		);

		SDL_SetRenderScale(m_Renderer, scale.X, scale.Y);
	}

	void SDLRendererAPI::DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size, Color color)
	{
		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		ChangeToSphynxCoords(center, m_Window);
		SDL_FRect rect = { (float)center.X, (float)center.Y, (float)size.X, (float)size.Y };

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

		DrawSDLPolygon(m_Renderer, drawMode, points, indices, color);
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
		float circumference = radius * (float)Math::TAU;
		float alpha = (float)Math::TAU / numSegments; // in radians
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

		DrawSDLPolygon(m_Renderer, drawMode, points, indices, color, true);
	}

	void SDLRendererAPI::DrawTexture(const Texture& texture, Vector2i position, Vector2i size, Color color)
	{
		ChangeToSphynxCoords(position, m_Window);
		SDL_FRect rect = { (float)position.X, (float)position.Y, (float)size.X, (float)size.Y };

		const SDLTexture* sdlTexture = static_cast<const SDLTexture*>(&texture);
		SDL_RenderTexture(m_Renderer, sdlTexture->GetTexture(), nullptr, &rect); // TODO: apply color
	}

	void SDLRendererAPI::DrawSprite(const Sprite& sprite, Vector2i position, Vector2i size, Color color)
	{
		Vector2i spritePosition = sprite.GetPosition();

		ChangeToSphynxCoords(position, m_Window);

		SDL_FRect dstRect = { (float)position.X, (float)position.Y, (float)size.X, (float)size.Y };
		SDL_FRect srcRect = { (float)spritePosition.X, (float)spritePosition.Y, (float)sprite.GetSize().X, (float)sprite.GetSize().Y };

		SDLTexture* sdlTexture = static_cast<SDLTexture*>(sprite.GetTexture());
		SDL_RenderTexture(m_Renderer, sdlTexture->GetTexture(), &srcRect, &dstRect); // TODO: apply color
	}

	void SDLRendererAPI::DrawText(const std::string& text, const Font& font, float size, Vector2i position, Vector2i scale, Color color)
	{
		// NOTE: SDL does not allow to get the font size, so we have to set it every time
		const SDLFont& sdlFont = static_cast<const SDLFont&>(font);

		TTF_Font* ttfFont = sdlFont.GetFont();
		if (ttfFont == nullptr) return;
		TTF_SetFontSize(ttfFont, (int)size);

		// create surface
		SDL_Surface* surfaceText = TTF_RenderText_Solid(ttfFont, text.c_str(), { color.R, color.G, color.B, color.A });

		// convert text into a texture
		SDL_Texture* textureText = SDL_CreateTextureFromSurface(m_Renderer, surfaceText);
		float w, h;
		SDL_GetTextureSize(textureText, &w, &h);

		// render textureText into a rectangle in the screen
		SDL_FRect rect = { (float)position.X, (float)position.Y, (float)scale.X * w, (float)scale.Y * h };
		SDL_RenderTexture(m_Renderer, textureText, NULL, &rect);

		// free surface, texture and font
		SDL_DestroySurface(surfaceText);
		SDL_DestroyTexture(textureText);
	}

	void SDLRendererAPI::DrawLine(const Transform& transform, Vector2f point1, Vector2f point2, float lineWidth, Color color)
	{
		if (lineWidth <= 0.0f) return;

		// multiply transformation matrices
		glm::mat4 mvpMatrix = GetMVPMatrix(transform);

		glm::vec4 P1 = mvpMatrix * glm::vec4{ point1.X, point1.Y, 0.0f, 1.0f };
		glm::vec4 P2 = mvpMatrix * glm::vec4{ point2.X, point2.Y, 0.0f, 1.0f };

		Vector2f scale{};
		SDL_GetRenderScale(m_Renderer, &scale.X, &scale.Y);
		SDL_SetRenderScale(m_Renderer, lineWidth, lineWidth);

		SDL_SetRenderDrawColor(m_Renderer, color.R, color.G, color.B, color.A);

		SDL_COORDS_TO_SPHYNX_COORDS(P1, m_Window);
		SDL_COORDS_TO_SPHYNX_COORDS(P2, m_Window);
		SDL_RenderLine(m_Renderer,
			(float)P1.x / lineWidth, (float)P1.y / lineWidth,
			(float)P2.x / lineWidth, (float)P2.y / lineWidth
		);

		SDL_SetRenderScale(m_Renderer, scale.X, scale.Y);
	}

	void SDLRendererAPI::DrawQuad(DrawMode drawMode, const Transform& transform, Vector2f size, Vector2f pivot, Color color)
	{
		// multiply transformation matrices
		glm::mat4 mvpMatrix = GetMVPMatrix(transform);

		// Coordinates in screen space
		glm::vec4 UL = mvpMatrix * glm::vec4{ -size.X * pivot.X		    ,  size.Y * (1.0f - pivot.Y), 0.0f, 1.0f };
		glm::vec4 UR = mvpMatrix * glm::vec4{ size.X * (1.0f - pivot.X),  size.Y * (1.0f - pivot.Y), 0.0f, 1.0f };
		glm::vec4 DR = mvpMatrix * glm::vec4{ size.X * (1.0f - pivot.X), -size.Y * pivot.Y		    , 0.0f, 1.0f };
		glm::vec4 DL = mvpMatrix * glm::vec4{ -size.X * pivot.X		    , -size.Y * pivot.Y		    , 0.0f, 1.0f };

		// define points and indices to draw
		std::vector<SDL_FPoint> points;
		points.reserve(5);
		points.emplace_back(SDL_FPoint{ UL.x, UL.y });
		points.emplace_back(SDL_FPoint{ UR.x, UR.y });
		points.emplace_back(SDL_FPoint{ DR.x, DR.y });
		points.emplace_back(SDL_FPoint{ DL.x, DL.y });
		points.emplace_back(SDL_FPoint{ UL.x, UL.y });

		for (int i = 0; i < points.size(); ++i)
		{
			SDL_COORDS_TO_SPHYNX_COORDS(points[i], m_Window);
		}

		std::vector<int32_t> indices({ 0, 1, 2, 2, 3, 0 });

		DrawSDLPolygon(m_Renderer, drawMode, points, indices, color);
	}

	void SDLRendererAPI::DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2f point1, Vector2f point2, Vector2f point3, Vector2f pivot, Color color)
	{
		glm::mat4 mvpMatrix = GetMVPMatrix(transform);
		float width = std::max({ point1.X, point2.X, point3.X }) - std::min({ point1.X, point2.X, point3.X });
		float height = std::max({ point1.Y, point2.Y, point3.Y }) - std::min({ point1.Y, point2.Y, point3.Y });

		// NOTE: maybe should be relative to the triangle center
		float xDiff = pivot.X * width - (1.0f - pivot.X) * width;
		float yDiff = pivot.Y * height - (1.0f - pivot.Y) * height;

		glm::vec4 P1 = mvpMatrix * glm::vec4{ point1.X - xDiff, point1.Y - yDiff, 0.0f, 1.0f };
		glm::vec4 P2 = mvpMatrix * glm::vec4{ point2.X - xDiff, point2.Y - yDiff, 0.0f, 1.0f };
		glm::vec4 P3 = mvpMatrix * glm::vec4{ point3.X - xDiff, point3.Y - yDiff, 0.0f, 1.0f };

		std::vector<SDL_FPoint> points;
		points.reserve(4);
		points.emplace_back(SDL_FPoint{ (float)P1.x, (float)P1.y });
		points.emplace_back(SDL_FPoint{ (float)P2.x, (float)P2.y });
		points.emplace_back(SDL_FPoint{ (float)P3.x, (float)P3.y });
		points.emplace_back(SDL_FPoint{ (float)P1.x, (float)P1.y });

		for (int i = 0; i < points.size(); ++i)
		{
			SDL_COORDS_TO_SPHYNX_COORDS(points[i], m_Window);
		}

		std::vector<int32_t> indices({ 0, 1, 2 });

		DrawSDLPolygon(m_Renderer, drawMode, points, indices, color);
	}

	void SDLRendererAPI::DrawCircle(DrawMode drawMode, const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot, Color color)
	{
		glm::mat4 mvpMatrix = GetMVPMatrix(transform);

		float xDiff = pivot.X * radius * 2.0f - (1.0f - pivot.X) * radius * 2.0f;
		float yDiff = pivot.Y * radius * 2.0f - (1.0f - pivot.Y) * radius * 2.0f;

		glm::vec4 center = mvpMatrix * glm::vec4{ -xDiff, -yDiff, 0.0f, 1.0f };
		SDL_COORDS_TO_SPHYNX_COORDS(center, m_Window);

		std::vector<SDL_FPoint> points; // reserve space for "numSegments" number of points
		points.reserve(numSegments + 2);
		points.emplace_back(SDL_FPoint{ center.x, center.y }); // push the center point

		std::vector<int32_t> indices;
		indices.reserve((numSegments + 1) * 3); // plus one to count the center

		// compute points and indices
		float circumference = radius * (float)Math::TAU;
		float alpha = (float)Math::TAU / numSegments; // in radians
		for (uint32_t i = 0; i < numSegments; ++i)
		{
			// push segments' first point
			float x = radius * std::cos(alpha * i);
			float y = radius * std::sin(alpha * i);

			glm::vec4 P = mvpMatrix * glm::vec4{ x - xDiff, y - yDiff, 0.0f, 1.0f };
			SDL_COORDS_TO_SPHYNX_COORDS(P, m_Window);

			points.emplace_back(SDL_FPoint{ P.x, P.y });
			if (i == numSegments - 1) points.emplace_back(points[1]);

			// push segment indices
			indices.emplace_back(0);
			indices.emplace_back(i + 1);
			indices.emplace_back(((i + 1) % numSegments) + 1);
		}

		DrawSDLPolygon(m_Renderer, drawMode, points, indices, color, true);
	}

	void SDLRendererAPI::DrawTexture(const Texture& texture, const Transform& transform, Vector2f size, Vector2f pivot, Color color)
	{
		// multiply transformation matrices
		glm::mat4 mvpMatrix = GetMVPMatrix(transform);

		// Coordinates in screen space
		glm::vec4 UL = mvpMatrix * glm::vec4{ -size.X * pivot.X		    ,  size.Y * (1.0f - pivot.Y), 0.0f, 1.0f };
		glm::vec4 UR = mvpMatrix * glm::vec4{ size.X * (1.0f - pivot.X),  size.Y * (1.0f - pivot.Y), 0.0f, 1.0f };
		glm::vec4 DR = mvpMatrix * glm::vec4{ size.X * (1.0f - pivot.X), -size.Y * pivot.Y		    , 0.0f, 1.0f };
		glm::vec4 DL = mvpMatrix * glm::vec4{ -size.X * pivot.X		    , -size.Y * pivot.Y		    , 0.0f, 1.0f };

		// define points and indices to draw
		std::vector<SDL_FPoint> points;
		points.reserve(4);
		points.emplace_back(SDL_FPoint{ UL.x, UL.y });
		points.emplace_back(SDL_FPoint{ UR.x, UR.y });
		points.emplace_back(SDL_FPoint{ DR.x, DR.y });
		points.emplace_back(SDL_FPoint{ DL.x, DL.y });

		for (int i = 0; i < points.size(); ++i)
		{
			SDL_COORDS_TO_SPHYNX_COORDS(points[i], m_Window);
		}

		std::vector<int32_t> indices({ 0, 1, 2, 2, 3, 0 });
		std::vector<float> uv({ 0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,  0.0f,1.0f });  // example: 1,0 -> uv for UR

		const SDLTexture* texture_SDL = static_cast<const SDLTexture*>(&texture);

		DrawSDLTexture(m_Renderer, texture_SDL, points, indices, uv, color);
	}

	void SDLRendererAPI::DrawSprite(const Sprite& sprite, const Transform& transform, Color color)
	{
		// multiply transformation matrices
		glm::mat4 mvpMatrix = GetMVPMatrix(transform);

		// calculate rectangle corners with transformations applied
		Vector2f pivot = sprite.GetPivot();
		Vector2f size = { (float)sprite.GetSize().X / sprite.GetPixelsPerUnit(), (float)sprite.GetSize().Y / sprite.GetPixelsPerUnit() };

		// Coordinates in screen space
		glm::vec4 UL = mvpMatrix * glm::vec4{ -size.X * pivot.X		    ,  size.Y * (1.0f - pivot.Y), 0.0f, 1.0f };
		glm::vec4 UR = mvpMatrix * glm::vec4{ size.X * (1.0f - pivot.X),  size.Y * (1.0f - pivot.Y), 0.0f, 1.0f };
		glm::vec4 DR = mvpMatrix * glm::vec4{ size.X * (1.0f - pivot.X), -size.Y * pivot.Y		    , 0.0f, 1.0f };
		glm::vec4 DL = mvpMatrix * glm::vec4{ -size.X * pivot.X		    , -size.Y * pivot.Y		    , 0.0f, 1.0f };

		// define points and indices to draw
		std::vector<SDL_FPoint> points;
		points.reserve(4);
		points.emplace_back(SDL_FPoint{ UL.x, UL.y });
		points.emplace_back(SDL_FPoint{ UR.x, UR.y });
		points.emplace_back(SDL_FPoint{ DR.x, DR.y });
		points.emplace_back(SDL_FPoint{ DL.x, DL.y });

		for (int i = 0; i < points.size(); ++i)
		{
			SDL_COORDS_TO_SPHYNX_COORDS(points[i], m_Window);
		}

		std::vector<int32_t> indices({ 0, 1, 2, 2, 3, 0 });

		float minU = (float)sprite.GetPosition().X / sprite.GetTexture()->GetWidth();
		float minV = (float)sprite.GetPosition().Y / sprite.GetTexture()->GetHeight();
		float maxU = (float)(sprite.GetPosition().X + sprite.GetSize().X) / sprite.GetTexture()->GetWidth();
		float maxV = (float)(sprite.GetPosition().Y + sprite.GetSize().Y) / sprite.GetTexture()->GetHeight();

		// example: uv for each corner of the sprite, in relation to the texture
		// if the size of a texture is (200, 100) and a one of the corners
		std::vector<float> uv({ minU,minV,  maxU,minV,  maxU,maxV,  minU,maxV });

		SDLTexture* texture_SDL = static_cast<SDLTexture*>(sprite.GetTexture());

		DrawSDLTexture(m_Renderer, texture_SDL, points, indices, uv, color);
	}

	SDL_Renderer* SDLRendererAPI::GetCurrentSDLRenderer() const
	{
		if (SDL_Texture* target = SDL_GetRenderTarget(m_Renderer))
		{
			return SDL_GetRendererFromTexture(target);
		}
		return m_Renderer;
	}
}