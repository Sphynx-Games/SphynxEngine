#pragma once

#include "Core/Core.h"
#include "RendererTypes.h"
#include "Math/Vector.h"
#include <string>


namespace Sphynx
{
	struct Camera;
	struct Transform;
	class RendererAPI;
	class Texture;
	class Sprite;
	class Font;

	class SPHYNX_API Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void Begin(const Camera* camera);
		static void End();

		static RendererAPI* GetRendererAPI();
		static const RendererConfig& GetConfiguration();

		static void SetViewport(Vector2i position, Vector2i size);
		static Vector2i GetViewportSize();
		static void SetClearColor(Color color);
		static void SetDrawMode(DrawMode drawMode);

		/* Screen space draw commands */
		static void DrawPoint(Vector2i point, Color color = Color::White);
		static void DrawLine(Vector2i point1, Vector2i point2, float lineWidth = 1.0f, Color color = Color::White);
		static void DrawQuad(Vector2i center, Vector2i size = { 100, 100 }, Color color = Color::White);
		static void DrawTriangle(Vector2i point1, Vector2i point2, Vector2i point3, Color color = Color::White);
		static void DrawCircle(Vector2i center, float radius = 50.0f, uint32_t numSegments = 32u, Color color = Color::White);

		static void DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size = { 100, 100 }, Color color = Color::White);
		static void DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color = Color::White);
		static void DrawCircle(DrawMode drawMode, Vector2i center, float radius = 50.0f, uint32_t numSegments = 32u, Color color = Color::White);

		static void DrawTexture(const Texture& texture, Vector2i position, Vector2i size = { 100, 100 }, Color color = Color::White);
		static void DrawSprite(const Sprite& sprite, Vector2i position, Vector2i size = { 100, 100 }, Color color = Color::White);
		static void DrawText(const std::string& text, const Font& font, uint8_t size, Vector2i position, Vector2i scale = { 1, 1 }, Color color = Color::White);

		/* World space draw commands */
		static void DrawLine(const Transform& transform, Vector2f point1, Vector2f point2, float lineWidth = 1.0f, Color color = Color::White);

		static void DrawQuad(const Transform& transform, Vector2f size = { 1.0f, 1.0f }, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawTriangle(const Transform& transform, Vector2f point1, Vector2f point2, Vector2f point3, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawCircle(const Transform& transform, float radius = 0.5f, uint32_t numSegments = 32u, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);

		static void DrawQuad(DrawMode drawMode, const Transform& transform, Vector2f size = { 1.0f, 1.0f }, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2f point1, Vector2f point2, Vector2f point3, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawCircle(DrawMode drawMode, const Transform& transform, float radius = 0.5f, uint32_t numSegments = 32u, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);

		static void DrawTexture(const Texture& texture, const Transform& transform, Vector2f size = { 1.0f, 1.0f }, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawSprite(const Sprite& sprite, const Transform& transform, Color color = Color::White);

	private:
		static RendererAPI* s_RendererAPI;
		static RendererConfig s_RendererConfig;
	};
}