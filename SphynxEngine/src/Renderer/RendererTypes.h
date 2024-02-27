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

		glm::mat4 ViewProjectionMatrix;  // POR QUÉ NO USAR UNA REFERENCIA DE LA PROPIA CÁMARA?
	};

	/*struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};*/
}