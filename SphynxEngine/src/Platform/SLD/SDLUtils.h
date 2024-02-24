#pragma once

#include "Renderer/Window.h"
#include "Math/Vector.h"

namespace Sphynx
{
	inline void ChangeToSphynxCoords(Vector2i& point, const Window* window)
	{
		point.Y = window->GetHeight() - point.Y;
	}
}