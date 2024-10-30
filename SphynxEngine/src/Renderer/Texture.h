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
	protected:
		Texture() : m_Size(0) {};
		Texture(const Vector2i& size) : m_Size(size) {};
	public:
		virtual ~Texture() = default;

		virtual Vector2i GetSize() const { return m_Size; }
		virtual uint32_t GetWidth() const { return m_Size.X; }
		virtual uint32_t GetHeight() const { return m_Size.Y; }

		virtual void* GetNativeTexture() const = 0;

	protected:
		Vector2i m_Size;

	public:
		static Texture* Create(void* data, Vector2i size);

	};
}