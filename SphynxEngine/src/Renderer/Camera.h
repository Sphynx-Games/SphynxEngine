#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>


namespace Sphynx
{
	struct SPHYNX_API Camera
	{
	public:
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;

		glm::mat4 ViewProjectionMatrix;
	};
}