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

	class SPHYNX_API FontLoader
	{
	public:
		static Font* Load(const std::filesystem::path& path);

	};
}

#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS(Sphynx::Font, SPHYNX_API)
