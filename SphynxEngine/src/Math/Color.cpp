#include "Color.h"

namespace Sphynx
{
	const Color Color::White = Color(255, 255, 255, 255);
	const Color Color::Black = Color(0, 0, 0, 255);

	Color::Color() : R(255), G(255), B(255), A(255) {}

	Color::Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A) : R(R), G(G), B(B), A(A) {}

	Color::Color(uint32_t hexColor) :
		R((hexColor >> 24) & 0xFF),
		G((hexColor >> 16) & 0xFF),
		B((hexColor >> 8) & 0xFF),
		A(hexColor & 0xFF)
	{}

	uint32_t Color::ToHex(Color color)
	{
		return (uint32_t(color.R) << 24) | (uint32_t(color.G) << 16) | (uint32_t(color.B) << 8) | uint32_t(color.A);
	}

	Color Color::FromHex(uint32_t color)
	{
		return color;
	}
}