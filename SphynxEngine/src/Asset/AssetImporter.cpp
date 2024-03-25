#include "spxpch.h"
#include "AssetImporter.h"
#include "AssetMetadata.h"


namespace Sphynx
{
	std::unordered_map<AssetType, AssetImportFunction> AssetImporter::s_AssetImportFunctions = {};

	std::shared_ptr<IAsset> AssetImporter::Import(const AssetMetadata& metadata)
	{
		return GetImporter(metadata.Type)(metadata);
	}

	void AssetImporter::RegisterImporter(const AssetType& type, AssetImportFunction importer)
	{
		SPX_CORE_ASSERT(!IsImporterRegistered(type), "There is already an importer registered for: {}", type);
		s_AssetImportFunctions[type] = importer;
	}

	bool AssetImporter::IsImporterRegistered(const AssetType& type)
	{
		return s_AssetImportFunctions.find(type) != s_AssetImportFunctions.end();
	}

	AssetImportFunction AssetImporter::GetImporter(const AssetType& type)
	{
		SPX_CORE_ASSERT(IsImporterRegistered(type), "There is no valid importer registered for: {}", type);
		return s_AssetImportFunctions[type];
	}
}