#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Asset.h"
#include "AssetMetadata.h"


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
		static std::unordered_map<AssetType, AssetImportFunction> s_AssetImportFunctions;

	};
}
