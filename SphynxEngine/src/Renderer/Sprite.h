#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"
#include "Asset/Asset.h"

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

namespace Sphynx
{
	class Texture;

	class SPHYNX_API Sprite
	{
	public:
		Sprite() = default;
		//std::shared_ptr<Asset<Texture>> texture
		Sprite(Texture* texture, Vector2i position = { 0, 0 }, Vector2i size = { 1, 1 }, Vector2f pivot = { 0.5f, 0.5f }, uint32_t pixelsPerUnit = 100);
		~Sprite() = default;

		//Texture* GetTexture() const { return m_Texture->Asset; }
		Texture* GetTexture() const { return m_Texture; }
		Vector2i GetPosition() const { return m_Position; }
		Vector2i GetSize() const { return m_Size; }
		Vector2f GetPivot() const { return m_Pivot; }
		uint32_t GetPixelsPerUnit() const { return m_PixelsPerUnit; }

		void SetPosition(Vector2i position) { m_Position = position; }
		void SetSize(Vector2i size) { m_Size = size; }
		void SetPivot(Vector2f pivot) { m_Pivot = pivot; }
		void SetPixelsPerUnit(uint32_t pixelsPerUnit)
		{
			SPX_CORE_ASSERT(pixelsPerUnit > 0, "Pixels per unit should be greater than 0!");
			m_PixelsPerUnit = pixelsPerUnit;
		}

	public:
		Texture* m_Texture;
		//std::shared_ptr<Asset<Texture>> m_Texture;
		Vector2i m_Position;
		Vector2i m_Size;
		Vector2f m_Pivot;
		uint32_t m_PixelsPerUnit;

		friend class SpriteAssetImporter;
	};

	class Spritesheet
	{
	public:
		Spritesheet(Texture* texture, int rows, int columns);
		~Spritesheet();

		Sprite* GetSprite(uint32_t num) { return m_Sprites.at(num); }

	private:
		Texture* m_Texture;
		std::vector<Sprite*> m_Sprites;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(Sphynx::Sprite)

SPX_REFLECT_PROPERTY(m_Position)
SPX_REFLECT_PROPERTY(m_Size)
SPX_REFLECT_PROPERTY(m_Pivot)
SPX_REFLECT_PROPERTY(m_PixelsPerUnit)

SPX_REFLECT_CLASS_END(Sphynx::Sprite)