#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	class SPHYNX_API Font
	{
	public:
		virtual ~Font() = default;

	public:
		static Font* Create();

	};
}