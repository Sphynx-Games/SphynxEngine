#include "spxpch.h"
#include "AssetImporter.h"
#include "AssetMetadata.h"


namespace Sphynx
{
	HashMap<AssetType, AssetImportFunction> AssetImporter::s_AssetImportFunctions = {};
	HashMap<AssetType, AssetLoadFunction> AssetImporter::s_AssetLoadFunctions = {};
	HashMap<AssetType, AssetSaveFunction> AssetImporter::s_AssetSaveFunctions = {};

	std::shared_ptr<IAsset> AssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		AssetImportFunction function = GetImporter(metadata.Type);
		return function(metadata, path);
	}

	std::shared_ptr<IAsset> AssetImporter::Load(const AssetMetadata& metadata)
	{
		AssetLoadFunction function = GetLoader(metadata.Type);
		return function(metadata);
	}

	void AssetImporter::Save(const AssetMetadata& metadata)
	{
		AssetSaveFunction function = GetSaver(metadata.Type);
		function(metadata);
	}

	void AssetImporter::RegisterImporter(const AssetType& type, AssetImportFunction importer)
	{
		SPX_CORE_ASSERT(!IsImporterRegistered(type), "There is already an importer registered for: {}", type);
		s_AssetImportFunctions[type] = importer;
	}

	void AssetImporter::RegisterLoader(const AssetType& type, AssetLoadFunction loader)
	{
		SPX_CORE_ASSERT(!IsLoaderRegistered(type), "There is already an importer registered for: {}", type);
		s_AssetLoadFunctions[type] = loader;
	}

	void AssetImporter::RegisterSaver(const AssetType& type, AssetSaveFunction saver)
	{
		SPX_CORE_ASSERT(!IsSaverRegistered(type), "There is already an importer registered for: {}", type);
		s_AssetSaveFunctions[type] = saver;
	}

	bool AssetImporter::IsImporterRegistered(const AssetType& type)
	{
		return s_AssetImportFunctions.ContainsKey(type);
	}

	bool AssetImporter::IsLoaderRegistered(const AssetType& type)
	{
		return s_AssetLoadFunctions.ContainsKey(type);
	}

	bool AssetImporter::IsSaverRegistered(const AssetType& type)
	{
		return s_AssetSaveFunctions.ContainsKey(type);
	}

	AssetImportFunction AssetImporter::GetImporter(const AssetType& type)
	{
		SPX_CORE_ASSERT(IsImporterRegistered(type), "There is no valid importer registered for: {}", type);
		return s_AssetImportFunctions[type];
	}

	AssetLoadFunction AssetImporter::GetLoader(const AssetType& type)
	{
		SPX_CORE_ASSERT(IsLoaderRegistered(type), "There is no valid loader registered for: {}", type);
		return s_AssetLoadFunctions[type];
	}

	AssetSaveFunction AssetImporter::GetSaver(const AssetType& type)
	{
		SPX_CORE_ASSERT(IsSaverRegistered(type), "There is no valid saver registered for: {}", type);
		return s_AssetSaveFunctions[type];
	}
}