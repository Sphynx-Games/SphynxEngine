#pragma once

#include "Asset/AssetManager.h"
#include "Renderer/Font.h"
#include "FontAssetImporter.h"


namespace Sphynx
{
	REGISTER_ASSET_TYPE(Font, &FontAssetImporter::Import, ".ttf");
}
