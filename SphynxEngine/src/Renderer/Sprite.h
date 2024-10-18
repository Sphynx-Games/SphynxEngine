#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"
#include "Container/Array.h"
#include "Asset/Asset.h"

#include <cstdint>
#include <string>
#include <memory>


namespace Sphynx
{
	class Texture;

	class SPHYNX_API Sprite
	{
	public:
		Sprite() = default;
		Sprite(Texture* texture, Vector2i position = { 0, 0 }, Vector2i size = { 1, 1 }, Vector2f pivot = { 0.5f, 0.5f }, uint32_t pixelsPerUnit = 100);
		~Sprite() = default;

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

	private:
		Texture* m_Texture;
		Vector2i m_Position;
		Vector2i m_Size;
		Vector2f m_Pivot;
		uint32_t m_PixelsPerUnit;

		friend class SpriteAssetImporter;
	};

	class Spritesheet
	{
	public:
		Spritesheet() = default;
		Spritesheet(Texture* texture);
		~Spritesheet();

		Texture* GetTexture() const { return m_Texture; }
		Sprite* GetSprite(uint32_t num) const { return m_Sprites.Get(num); }

		Sprite* AddSprite(Vector2i position, Vector2i size);
		static Spritesheet* Create(Texture* texture, int rows, int columns);

	private:
		Texture* m_Texture;
		Array<Sprite*> m_Sprites;

		friend class SpriteAssetImporter;
		friend class SpritesheetAssetImporter;
	};
}