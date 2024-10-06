#pragma once

#include "Asset/AssetManager.h"
#include "Renderer/Sprite.h"
#include "SpriteAssetImporter.h"


namespace Sphynx
{
	REGISTER_ASSET_TYPE(Sprite, &SpriteAssetImporter::Import, ".png", ".jpg", "jpeg");
}
