#pragma once

#include "Core/Core.h"
#include "Math/Color.h"

#include <glm/glm.hpp>

namespace Sphynx
{
	enum SPHYNX_API DrawMode
	{
		WIREFRAME,
		FILLED
	};

	struct RendererConfig
	{
		DrawMode DrawMode = DrawMode::WIREFRAME;
		Color ClearColor = Color::Black;

		// If scene data grows, then add this to a different struct
		// such as: "SceneData"
		glm::mat4 ViewProjectionMatrix;
	};
}