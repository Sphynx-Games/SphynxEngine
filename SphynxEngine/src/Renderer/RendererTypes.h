#pragma once

#include "Math/Color.h"
#include <glm/ext/matrix_transform.hpp>

namespace Sphynx
{
	enum DrawMode
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