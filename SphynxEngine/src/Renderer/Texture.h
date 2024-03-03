#pragma once

#include "Math/Vector.h"
#include <cstdint>
#include <string>
#include <memory>

namespace Sphynx
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() { return m_Size.X; }
		virtual uint32_t GetHeight() { return m_Size.Y; }

		virtual const std::string& GetPath() { return m_Path; }

	protected:
		std::string m_Path;
		Vector2i m_Size;

	public:
		static Texture* Create(const std::string& path);
	};
}