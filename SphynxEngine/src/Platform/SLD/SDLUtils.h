#pragma once

#include "Renderer/Window.h"

namespace Sphynx
{
	inline void ChangeToSphynxCoords(uint32_t &x, uint32_t &y, const Window* window)
	{
		y = window->GetHeight() - y;
	}
}