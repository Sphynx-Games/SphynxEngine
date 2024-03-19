#pragma once

#include "Renderer/RendererAPI.h"
#include <cstdint>

namespace Sphynx 
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

		//static void OnWindowResize(uint32_t width, uint32_t height);

		//static void Submit(const Shader& shader, const VertexArray& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static Renderer* Create() { return new Renderer(); };
	};
}