#include "spxpch.h"
#include "Sprite.h"
#include "Texture.h"

namespace Sphynx
{
	Sprite::Sprite(Texture* texture, Vector2i position, Vector2i size, Vector2f pivot, uint32_t pixelsPerUnit)
		: m_Texture(texture), m_Position(position), m_Size(size), m_Pivot(pivot), m_PixelsPerUnit(pixelsPerUnit)
	{
	}

	Spritesheet::Spritesheet(Texture* texture, int rows, int columns) : m_Texture(texture)
	{
		m_Sprites.reserve(rows * columns);
		Vector2i sizeSprite = Vector2i{ (int32_t)m_Texture->GetWidth() / columns, (int32_t)m_Texture->GetHeight() / rows };

		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				Vector2i position = { sizeSprite.X * j, sizeSprite.Y * i};
				m_Sprites.emplace_back(new Sprite(texture, position, sizeSprite));
			}
		}
	}

	Spritesheet::~Spritesheet()
	{
		for (Sprite* sprite : m_Sprites)
		{
			if (sprite != nullptr)
			{
				delete sprite;
			}
		}
		m_Sprites.clear();
	}
}