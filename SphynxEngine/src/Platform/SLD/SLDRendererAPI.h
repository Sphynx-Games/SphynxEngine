#pragma once

#include "Renderer/RendererAPI.h"

struct SDL_Renderer;

namespace Sphynx
{
	class SDLRendererAPI : public RendererAPI
	{
	public:
		SDLRendererAPI();
		virtual ~SDLRendererAPI();

		virtual void Init() override;
		virtual void SetViewport(Vector2i position, Vector2i size) override;
		virtual void Clear(Color color) override;
		virtual void Present() override;

		virtual void DrawPoint(Vector2i point, Color color) override;
		virtual void DrawLine(Vector2i point1, Vector2i point2, Color color) override;
		
		virtual void DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size, Color color) override;
		virtual void DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color) override;
		virtual void DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color) override;

		virtual void DrawQuad(DrawMode drawMode, const Transform& transform, Vector2i size, Vector2f pivot, Color color) override;
		virtual void DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2i point1, Vector2i point2, Vector2i point3, Vector2f pivot, Color color) override;
		virtual void DrawCircle(DrawMode drawMode, const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot, Color color) override;

		//virtual void SetLineWidth(float width) override;

	private:
		const class Window* m_Window;
		SDL_Renderer* m_Renderer;
	};
}