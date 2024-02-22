#pragma once

#include <cstdint>

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
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Clear(uint8_t R = 0, uint8_t G = 0, uint8_t B = 0, uint8_t A = 255) = 0;
		virtual void Present() = 0;
		
		virtual void DrawPoint(uint32_t x, uint32_t y, uint8_t R, uint8_t G, uint8_t B, uint8_t A) = 0;
		virtual void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t R, uint8_t G, uint8_t B, uint8_t A) = 0;
		virtual void DrawQuad(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t R, uint8_t G, uint8_t B, uint8_t A) = 0;
		virtual void DrawTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint8_t R, uint8_t G, uint8_t B, uint8_t A) = 0;
		virtual void DrawCircle(uint32_t x, uint32_t y, float radius, uint32_t numSegments, uint8_t R, uint8_t G, uint8_t B, uint8_t A) = 0;

		//virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
		static RendererAPI* Create();

	private:
		static API s_API;
	};
}