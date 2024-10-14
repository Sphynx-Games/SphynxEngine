#pragma once

#include "Core/Core.h"
#include "Asset.h"
#include "AssetMetadata.h"
#include "AssetImporter.h"
#include "Container/Map.h"

#include <string>
#include <set>
#include <memory>

#define ASSET_EXTENSION ".spxasset"


namespace Sphynx
{
	using AssetTypeRegistry = std::set<AssetType>;
	using AssetRegistry = HashMap<AssetHandle, AssetMetadata>;

	class AssetManager
	{
	public:
		static void Init();
		static void Shutdown();

		static void RegisterAssetType(const AssetType& assetType);
		static void RegisterAssetTypeExtensions(const AssetType& assetType, const std::initializer_list<std::string>& extensions);
		static bool IsAssetTypeRegistered(const AssetType& assetType);

		static std::shared_ptr<IAsset> Import(const std::filesystem::path& path);
		static std::shared_ptr<IAsset> GetAsset(AssetHandle handle);
		static const AssetMetadata& GetMetadata(AssetHandle handle);
		static bool IsAssetLoaded(AssetHandle handle);
		static AssetType GetAssetType(AssetHandle handle);
		static AssetType GetAssetTypeFromExtension(const std::string& extension);
		static AssetHandle GetAssetHandleFromAddress(void* address);

		template<typename T>
		static std::shared_ptr<Asset<T>> Import(const std::filesystem::path& path)
		{
			std::shared_ptr<IAsset> asset = Import(path);
			return std::static_pointer_cast<Asset<T>>(asset);
		}

		template<typename T>
		static std::shared_ptr<Asset<T>> GetAsset(AssetHandle handle)
		{
			std::shared_ptr<IAsset> asset = GetAsset(handle);
			return std::static_pointer_cast<Asset<T>>(asset);
		}

		template<typename T>
		static std::shared_ptr<IAsset> RegisterAsset(T* object, const std::filesystem::path& path)
		{
			SPX_CORE_ASSERT(!std::filesystem::is_directory(path), "The given path cannot be a directory!!");

			// create metadata and asset pointer
			AssetMetadata metadata;
			metadata.Handle = AssetHandle::Generate();
			metadata.Path = path;
			metadata.Path.replace_extension(ASSET_EXTENSION);
			metadata.Type = TypeToAssetType<T>::Value;

			std::shared_ptr<Asset<T>> asset = std::make_shared<Asset<T>>();
			asset->Handle = metadata.Handle;
			asset->Type = metadata.Type;
			asset->Asset = object;

			// make asset manager manage the asset and save data into .spxasset file
			ManageAsset(asset, metadata);
			AssetImporter::Save(metadata);

			return asset;
		}

	private:
		static void ManageAsset(std::shared_ptr<IAsset> asset, const AssetMetadata& metadata);

	private:
		static AssetTypeRegistry s_TypeRegistry;
		static AssetRegistry s_Registry;

	};
}

#define RegisterAssetType(Type) RegisterAssetType(#Type)
#define RegisterAssetTypeExtensions(Type, Extensions) RegisterAssetTypeExtensions(#Type, Extensions)
#define IsAssetRegistered(Type) IsAssetRegistered(#Type)
