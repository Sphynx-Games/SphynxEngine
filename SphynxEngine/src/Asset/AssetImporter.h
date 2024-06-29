#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Asset.h"
#include "AssetMetadata.h"
#include "Container/Map.h"


namespace Sphynx
{
	using AssetImportFunction = std::shared_ptr<IAsset>(*)(const AssetMetadata&);

	class AssetImporter
	{
	public:
		static std::shared_ptr<IAsset> Import(const AssetMetadata& metadata);

		static void RegisterImporter(const AssetType& type, AssetImportFunction importer);
		static bool IsImporterRegistered(const AssetType& type);
		static AssetImportFunction GetImporter(const AssetType& type);

	private:
		static HashMap<AssetType, AssetImportFunction> s_AssetImportFunctions;

	};
}
