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
		virtual void SetViewport(glm::vec2 position, uint32_t width, uint32_t height) override;
		virtual void Clear(glm::vec4 color) override;
		virtual void Present() override;

		virtual void DrawPoint(glm::vec2 point, glm::vec4 color) override;
		virtual void DrawLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color) override;
		virtual void DrawQuad(glm::vec2 point, uint32_t width, uint32_t height, glm::vec4 color) override;
		virtual void DrawTriangle(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3, glm::vec4 color) override;
		virtual void DrawCircle(glm::vec2 point, float radius, uint32_t numSegments, glm::vec4 color) override;

		//virtual void SetLineWidth(float width) override;

	private:
		const Window* m_Window;
		SDL_Renderer* m_Renderer;
	};
}