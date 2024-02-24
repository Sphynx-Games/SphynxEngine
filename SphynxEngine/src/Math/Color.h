#pragma once

#include "Core/Core.h"
#include <cstdint>

namespace Sphynx
{
	struct SPHYNX_API Color
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;
		uint8_t A;

		Color();
		Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
		Color(uint32_t hexColor);

		static uint32_t ToHex(Color color);
		static Color FromHex(uint32_t color);

		static const Color White;
		static const Color Black;
	};
}