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
		Texture(const Vector2u& size) : m_Size(size) {};
	public:
		virtual ~Texture() = default;

		virtual const Vector2u& GetSize() const { return m_Size; }
		virtual uint32_t GetWidth() const { return m_Size.X; }
		virtual uint32_t GetHeight() const { return m_Size.Y; }

		virtual void* GetNativeTexture() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

	protected:
		Vector2u m_Size;

	public:
		static Texture* Create(void* data, const Vector2u& size);

	};

	class SPHYNX_API TextureLoader
	{
	public:
		static Texture* Load(const std::filesystem::path& path);

	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_CLASS_BEGIN(Sphynx::Texture, SPHYNX_API)

//SPX_REFLECT_PROPERTY(m_Size)

SPX_REFLECT_CLASS_END(Sphynx::Texture, SPHYNX_API)