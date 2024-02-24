#pragma once

#include "Renderer/RendererAPI.h"

struct SDL_Renderer;

namespace Sphynx
{
	class Window;

	class SDLRendererAPI : public RendererAPI
	{
	public:
		SDLRendererAPI();
		virtual ~SDLRendererAPI();

		virtual void Init() override;
		virtual void SetViewport(Vector2i position, uint32_t width, uint32_t height) override;
		virtual void Clear(Color color) override;
		virtual void Present() override;

		virtual void DrawPoint(Vector2i point, Color color) override;
		virtual void DrawLine(Vector2i point1, Vector2i point2, Color color) override;
		virtual void DrawQuad(Vector2i point, uint32_t width, uint32_t height, Color color) override;
		virtual void DrawTriangle(Vector2i point1, Vector2i point2, Vector2i point3, Color color) override;
		virtual void DrawCircle(Vector2i center, float radius, uint32_t numSegments, Color color) override;

		virtual void DrawQuad(DrawMode drawMode, Vector2i point, uint32_t width, uint32_t height, Color color) override;
		virtual void DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color) override;
		virtual void DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color) override;

		//virtual void SetLineWidth(float width) override;

	private:
		const Window* m_Window;
		SDL_Renderer* m_Renderer;
	};
}