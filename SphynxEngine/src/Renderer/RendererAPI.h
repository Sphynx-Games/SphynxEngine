#pragma once

#include "Math/Vector.h"
#include "Math/Color.h"
#include "RendererTypes.h"
#include <string>


namespace Sphynx 
{
	struct Transform;
	class Texture;
	class Sprite;
	class Font;
	
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
		virtual void Shutdown() = 0;

		virtual void SetViewport(Vector2i position, Vector2i size) = 0;
		virtual void Clear(Color color) = 0;
		virtual void Present() = 0;

		/* Screen space draw commands */
		virtual void DrawPoint(Vector2i point, Color color) = 0;
		virtual void DrawLine(Vector2i point1, Vector2i point2, float lineWidth, Color color) = 0;
		virtual void DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size, Color color) = 0;
		virtual void DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color) = 0;
		virtual void DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color) = 0;
		virtual void DrawTexture(const Texture& texture, Vector2i position, Vector2i size, Color color) = 0;
		virtual void DrawSprite(const Sprite& sprite, Vector2i position, Vector2i size, Color color) = 0;
		virtual void DrawText(const std::string& text, const Font& font, float size, Vector2i position, Vector2i scale, Color color) = 0;

		/* World space draw commands */
		virtual void DrawLine(const Transform& transform, Vector2f point1, Vector2f point2, float lineWidth, Color color) = 0;
		virtual void DrawQuad(DrawMode drawMode, const Transform& transform, Vector2f size, Vector2f pivot, Color color) = 0;
		virtual void DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2f point1, Vector2f point2, Vector2f point3, Vector2f pivot, Color color) = 0;
		virtual void DrawCircle(DrawMode drawMode, const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot, Color color) = 0;
		virtual void DrawTexture(const Texture& texture, const Transform& transform, Vector2f size, Vector2f pivot, Color color) = 0;
		virtual void DrawSprite(const Sprite& sprite, const Transform& transform, Color color) = 0;

		//virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
		static RendererAPI* Create();

	private:
		static API s_API;
	};
}