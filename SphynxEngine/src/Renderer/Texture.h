#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"

#include <cstdint>
#include <string>
#include <memory>

namespace Sphynx
{
	class SPHYNX_API Texture
	{
	public:
		virtual ~Texture() = default;

		virtual Vector2i GetSize() const { return m_Size; }
		virtual uint32_t GetWidth() const { return m_Size.X; }
		virtual uint32_t GetHeight() const { return m_Size.Y; }

	protected:
		Vector2i m_Size;

	public:
		static Texture* Create(void* data, Vector2i size);

	};
}