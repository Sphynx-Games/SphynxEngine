#pragma once

#include "Asset/AssetManager.h"
#include "Renderer/Texture.h"
#include "TextureAssetImporter.h"


namespace Sphynx
{
	REGISTER_ASSET_TYPE(Texture, &TextureAssetImporter::Import, ".png", ".jpg", "jpeg");
}
