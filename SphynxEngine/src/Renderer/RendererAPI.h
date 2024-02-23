#pragma once

#include <glm/glm.hpp>

namespace Sphynx 
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, SDL = 1
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(glm::vec2 position, uint32_t width, uint32_t height) = 0;
		virtual void Clear(glm::vec4 color) = 0;
		virtual void Present() = 0;

		virtual void DrawPoint(glm::vec2 point, glm::vec4 color) = 0;
		virtual void DrawLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color) = 0;
		virtual void DrawQuad(glm::vec2 point, uint32_t width, uint32_t height, glm::vec4 color) = 0;
		virtual void DrawTriangle(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3, glm::vec4 color) = 0;
		virtual void DrawCircle(glm::vec2 point, float radius, uint32_t numSegments, glm::vec4 color) = 0;

		//virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
		static RendererAPI* Create();

	private:
		static API s_API;
	};
}