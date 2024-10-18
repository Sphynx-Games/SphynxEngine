#include "spxpch.h"
#include "Sprite.h"
#include "Texture.h"

namespace Sphynx
{
	Sprite::Sprite(Texture* texture, Vector2i position, Vector2i size, Vector2f pivot, uint32_t pixelsPerUnit) : 
		m_Texture(texture),
		m_Position(position),
		m_Size(size),
		m_Pivot(pivot),
		m_PixelsPerUnit(pixelsPerUnit)
	{
	}

	Spritesheet::Spritesheet(Texture* texture) :
		m_Texture(texture), m_Sprites()
	{
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
		m_Sprites.RemoveAll();
	}

	Sprite* Spritesheet::AddSprite(Vector2i position, Vector2i size)
	{
		Sprite* sprite = new Sprite(m_Texture, position, size);
		m_Sprites.Emplace(sprite);
		return sprite;
	}

	Spritesheet* Spritesheet::Create(Texture* texture, int rows, int columns)
	{
		Spritesheet* spritesheet = new Spritesheet(texture);
		Vector2i sizeSprite = Vector2i{ (int32_t)texture->GetWidth() / columns, (int32_t)texture->GetHeight() / rows };

		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				Vector2i position = { sizeSprite.X * j, sizeSprite.Y * i };
				spritesheet->m_Sprites.Emplace(new Sprite(texture, position, sizeSprite));
			}
		}
		return spritesheet;
	}
}