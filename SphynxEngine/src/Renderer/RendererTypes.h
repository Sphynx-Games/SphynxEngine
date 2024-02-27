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

		glm::mat4 ViewProjectionMatrix;  // POR QU� NO USAR UNA REFERENCIA DE LA PROPIA C�MARA?
	};

	/*struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};*/
}