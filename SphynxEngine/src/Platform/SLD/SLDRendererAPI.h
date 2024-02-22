#pragma once

#include "Renderer/RendererAPI.h"
#include <SDL.h>

namespace Sphynx
{
	class Window;

	class SDLRendererAPI : public RendererAPI
	{
	public:
		SDLRendererAPI();
		virtual ~SDLRendererAPI();

		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void Clear(uint8_t R = 0, uint8_t G = 0, uint8_t B = 0, uint8_t A = 255) override;
		virtual void Present() override;

		virtual void DrawPoint(uint32_t x, uint32_t y, uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;
		virtual void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;
		virtual void DrawQuad(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;
		virtual void DrawTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;
		virtual void DrawCircle(uint32_t x, uint32_t y, float radius, uint32_t numSegments, uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;

		//virtual void SetLineWidth(float width) override;

	private:
		const Window* m_Window;
		SDL_Renderer* m_Renderer;
	};
}