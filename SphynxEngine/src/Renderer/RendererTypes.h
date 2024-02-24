#pragma once

#include "Math/Color.h"

namespace Sphynx
{
	enum DrawMode
	{
		WIREFRAME, 
		FILLED
	};

	struct RendererConfig
	{
		Color ClearColor;
		DrawMode DrawMode;
	};
}