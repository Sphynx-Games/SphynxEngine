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
		explicit Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
		explicit Color(uint32_t hexColor);

		static uint32_t ToHex(Color color);
		static Color FromHex(uint32_t color);

		static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::Color)

SPX_REFLECT_PROPERTY(R)
SPX_REFLECT_PROPERTY(G)
SPX_REFLECT_PROPERTY(B)
SPX_REFLECT_PROPERTY(A)

SPX_REFLECT_STRUCT_END(Sphynx::Color)