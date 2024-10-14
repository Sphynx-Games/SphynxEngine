#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Asset.h"
#include "AssetMetadata.h"
#include "Container/Map.h"


namespace Sphynx
{
	using AssetImportFunction = std::shared_ptr<IAsset>(*)(const AssetMetadata&, const std::filesystem::path&);
	using AssetLoadFunction = std::shared_ptr<IAsset>(*)(const AssetMetadata&);
	using AssetSaveFunction = void(*)(const AssetMetadata&);

	class AssetImporter
	{
	public:
		static std::shared_ptr<IAsset> Import(const AssetMetadata& metadata, const std::filesystem::path& path);
		static std::shared_ptr<IAsset> Load(const AssetMetadata& metadata);
		static void Save(const AssetMetadata& metadata);

		static void RegisterImporter(const AssetType& type, AssetImportFunction importer);
		static void RegisterLoader(const AssetType& type, AssetLoadFunction loader);
		static void RegisterSaver(const AssetType& type, AssetSaveFunction saver);

		static bool IsImporterRegistered(const AssetType& type);
		static bool IsLoaderRegistered(const AssetType& type);
		static bool IsSaverRegistered(const AssetType& type);

		static AssetImportFunction GetImporter(const AssetType& type);
		static AssetLoadFunction GetLoader(const AssetType& type);
		static AssetSaveFunction GetSaver(const AssetType& type);

	private:
		static HashMap<AssetType, AssetImportFunction> s_AssetImportFunctions;
		static HashMap<AssetType, AssetLoadFunction> s_AssetLoadFunctions;
		static HashMap<AssetType, AssetSaveFunction> s_AssetSaveFunctions;

	};
}
