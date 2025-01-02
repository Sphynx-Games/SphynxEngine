#pragma once

#include "Core/Core.h"
#include "Renderer/RendererAPI.h"


namespace Sphynx
{
	class OpenGLShader;
	class OpenGLTexture;

	class SPHYNX_API OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI();
		virtual ~OpenGLRendererAPI() override;

		virtual void Init() override;
		virtual void Shutdown() override;

		/*virtual void OnWindowResized(uint32_t width, uint32_t height) override;

		virtual void SetClearColor(float r, float g, float b, float a) const override;
		virtual void Clear() const override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const override;*/

		void SetViewport(Vector2i position, Vector2i size) override;
		Vector2i GetViewportSize() override;
		void Clear(Color color) override;
		void Present() override;

		void DrawPoint(Vector2i point, Color color) override;
		void DrawLine(Vector2i point1, Vector2i point2, float lineWidth, Color color) override;
		void DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size, Color color) override;
		void DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color) override;
		void DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color) override;
		void DrawTexture(const Texture& texture, Vector2i position, Vector2i size, Color color) override;
		void DrawSprite(const Sprite& sprite, Vector2i position, Vector2i size, Color color) override;
		void DrawText(const std::string& text, const Font& font, float size, Vector2i position, Vector2i scale, Color color) override;

		void DrawLine(const Transform& transform, Vector2f point1, Vector2f point2, float lineWidth, Color color) override;
		void DrawQuad(DrawMode drawMode, const Transform& transform, Vector2f size, Vector2f pivot, Color color) override;
		void DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2f point1, Vector2f point2, Vector2f point3, Vector2f pivot, Color color) override;
		void DrawCircle(DrawMode drawMode, const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot, Color color) override;
		void DrawTexture(const Texture& texture, const Transform& transform, Vector2f size, Vector2f pivot, Color color) override;
		void DrawSprite(const Sprite& sprite, const Transform& transform, Color color) override;
	
	private:
		OpenGLShader* m_DefaultShader;
		OpenGLTexture* m_WhiteTexture;

	};
}

