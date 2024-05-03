#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"

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
		Sprite(Texture* texture, Vector2i position, Vector2i size, Vector2f pivot = { 0.5f, 0.5f }, uint32_t pixelsPerUnit = 100);
		~Sprite() = default;

		Texture* GetTexture() const { return m_Texture; }
		Vector2i GetPosition() const { return m_Position; }
		Vector2i GetSize() const { return m_Size; }
		Vector2f GetPivot() const { return m_Pivot; }
		uint32_t GetPixelsPerUnit() const { return m_PixelsPerUnit; }

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