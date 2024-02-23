#pragma once

#include "Renderer/Window.h"
#include <glm/glm.hpp>

namespace Sphynx
{
	inline void ChangeToSphynxCoords(glm::vec2& point, const Window* window)
	{
		point.y = window->GetHeight() - point.y;
	}
}