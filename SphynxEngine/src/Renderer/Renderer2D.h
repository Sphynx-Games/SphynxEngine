#pragma once

#include "RendererTypes.h"
#include "Math/Vector.h"
#include "Math/Transform.h"
//#include "Texture.h"

namespace Sphynx
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void Begin();
		static void End();

		static const RendererConfig& GetRendererConfig();

		static void SetViewport(Vector2i position, Vector2i size);
		static void SetClearColor(Color color);
		static void SetDrawMode(DrawMode drawMode);

		// ---- BASIC PRIMITIVES ----
		static void DrawPoint(Vector2i point, Color color = Color::White);
		static void DrawLine(Vector2i point1, Vector2i point2, Color color = Color::White);
		static void DrawQuad(Vector2i center, Vector2i size, Color color = Color::White);
		static void DrawTriangle(Vector2i point1, Vector2i point2, Vector2i point3, Color color = Color::White);
		static void DrawCircle(Vector2i center, float radius, uint32_t numSegments, Color color = Color::White);

		// with draw mode
		static void DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size, Color color = Color::White);
		static void DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color = Color::White);
		static void DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color = Color::White);

		// with transform
		static void DrawQuad(const Transform& transform, Vector2i size, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawTriangle(const Transform& transform, Vector2i point1, Vector2i point2, Vector2i point3, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawCircle(const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);

		// with draw mode and transform
		static void DrawQuad(DrawMode drawMode, const Transform& transform, Vector2i size, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2i point1, Vector2i point2, Vector2i point3, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);
		static void DrawCircle(DrawMode drawMode, const Transform& transform , float radius, uint32_t numSegments, Vector2f pivot = { 0.5f, 0.5f }, Color color = Color::White);

		// ---- TEXTURES ----
		//static void DrawTexture(const Texture2D& texture, Vector2i position, Vector2i size);
		//static void DrawTexture(const Texture2D& texture, Vector2i position, Vector2i size, const Transform& transform);

		/*static void DrawQuad(const Vector2i& position, const Vector2i& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const const glm::vec4&& tintColor = const glm::vec4&(1.0f));
		static void DrawQuad(const glm::vec3& position, const Vector2i& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const const glm::vec4&& tintColor = const glm::vec4&(1.0f));

		static void DrawQuad(const glm::mat4& transform, const const glm::vec4&& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const const glm::vec4&& tintColor = const glm::vec4&(1.0f), int entityID = -1);

		static void DrawRotatedQuad(const Vector2i& position, const Vector2i& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const const glm::vec4&& tintColor = const glm::vec4&(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const Vector2i& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const const glm::vec4&& tintColor = const glm::vec4&(1.0f));

		static void DrawCircle(const glm::mat4& transform, const const glm::vec4&& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);*/

		//static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

		/*struct TextParams
		{
			const glm::vec4& Color{ 1.0f };
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
		};
		static void DrawString(const std::string& string, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams, int entityID = -1);
		static void DrawString(const std::string& string, const glm::mat4& transform, const TextComponent& component, int entityID = -1);*/

		//static void SetLineWidth(float width);

	private:
		static class RendererAPI* s_RendererAPI;
		static RendererConfig s_RendererConfig;
	};
}