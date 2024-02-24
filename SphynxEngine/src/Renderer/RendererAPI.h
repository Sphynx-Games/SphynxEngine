#pragma once

#include "Math/Vector.h"
#include "Math/Color.h"
#include "RendererTypes.h"

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
		virtual void SetViewport(Vector2i position, uint32_t width, uint32_t height) = 0;
		virtual void Clear(Color color) = 0;
		virtual void Present() = 0;

		virtual void DrawPoint(Vector2i point, Color color) = 0;
		virtual void DrawLine(Vector2i point1, Vector2i point2, Color color) = 0;
		virtual void DrawQuad(Vector2i point, uint32_t width, uint32_t height, Color color) = 0;
		virtual void DrawTriangle(Vector2i point1, Vector2i point2, Vector2i point3, Color color) = 0;
		virtual void DrawCircle(Vector2i center, float radius, uint32_t numSegments, Color color) = 0;

		virtual void DrawQuad(DrawMode drawMode, Vector2i point, uint32_t width, uint32_t height, Color color) = 0;
		virtual void DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color) = 0;
		virtual void DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color) = 0;

		//virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
		static RendererAPI* Create();

	private:
		static API s_API;
	};
}