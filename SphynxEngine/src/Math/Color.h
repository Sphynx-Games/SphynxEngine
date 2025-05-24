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

		static constexpr uint32_t White		=	0xFFFFFFFF;
		static constexpr uint32_t Black		=	0x000000FF;
		static constexpr uint32_t Red		=	0xFF0000FF;
		static constexpr uint32_t Green		=	0x00FF00FF;
		static constexpr uint32_t Blue		=	0x0000FFFF;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::Color, SPHYNX_API)

SPX_REFLECT_PROPERTY(R)
SPX_REFLECT_PROPERTY(G)
SPX_REFLECT_PROPERTY(B)
SPX_REFLECT_PROPERTY(A)

SPX_REFLECT_STRUCT_END(Sphynx::Color, SPHYNX_API)