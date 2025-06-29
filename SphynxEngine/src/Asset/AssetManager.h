#pragma once

#include "Core/Core.h"
#include "Asset.h"
#include "AssetMetadata.h"
#include "AssetImporter.h"
#include "Container/Map.h"
#include "Container/Array.h"

#include <string>
#include <set>
#include <memory>
#include <filesystem>

#define ASSET_EXTENSION ".spxasset"


namespace Sphynx
{
	using AssetTypeRegistry = std::set<AssetType>;
	template class SPHYNX_API Map<AssetHandle, AssetMetadata>;
	using AssetRegistry = Map<AssetHandle, AssetMetadata>;

	class SPHYNX_API AssetManager
	{
	public:
		static void Init();
		static void Shutdown();

		static void RegisterAssetType(const AssetType& assetType);
		static void RegisterAssetTypeExtensions(const AssetType& assetType, const std::initializer_list<std::string>& extensions);
		static void UnregisterAssetType(const AssetType& assetType);
		static void UnregisterAssetTypeExtensions(const AssetType& assetType);
		static bool IsAssetTypeRegistered(const AssetType& assetType);

		static void AddToRegistry(const AssetMetadata& metadata);
		static void RemoveFromRegistry(AssetHandle handle);
		static const AssetRegistry& GetRegistry();

		static std::shared_ptr<IAsset> Import(const std::filesystem::path& path);
		static std::shared_ptr<IAsset> GetAsset(AssetHandle handle);
		static void UnloadAsset(AssetHandle handle);
		static void UnloadAssets();
		static const AssetMetadata& GetAssetMetadata(AssetHandle handle);
		static const AssetMetadata& GetAssetMetadataFromPath(const std::filesystem::path& path);
		static bool IsAssetLoaded(AssetHandle handle);
		static AssetType GetAssetType(AssetHandle handle);
		static AssetType GetAssetTypeFromExtension(const std::string& extension);
		static AssetHandle GetAssetHandleFromAddress(void* address);
		static AssetHandle GetAssetHandleFromPath(const std::filesystem::path& path);

		template<typename T>
		static std::shared_ptr<Asset<T>> Import(const std::filesystem::path& path);

		template<typename T>
		static std::shared_ptr<Asset<T>> GetAsset(AssetHandle handle);

		template<typename T>
		static Array<AssetHandle> GetAssetHandleList();

		template<typename T>
		static Array<AssetMetadata> GetAssetMetadatas();

		static Array<AssetMetadata> GetAssetMetadatasByAssetType(const AssetType& assetType);

		template<typename T>
		static std::shared_ptr<Asset<T>> RegisterAsset(T* object, const std::filesystem::path& path, bool save = true);		

	private:
		static void ManageAsset(std::shared_ptr<IAsset> asset, const AssetMetadata& metadata);

	private:
		static AssetTypeRegistry s_TypeRegistry;
		static AssetRegistry s_Registry;

	};	

	template<typename T>
	inline static std::shared_ptr<Asset<T>> AssetManager::Import(const std::filesystem::path& path)
	{
		std::shared_ptr<IAsset> asset = Import(path);
		return std::static_pointer_cast<Asset<T>>(asset);
	}

	template<typename T>
	inline static std::shared_ptr<Asset<T>> AssetManager::GetAsset(AssetHandle handle)
	{
		std::shared_ptr<IAsset> asset = GetAsset(handle);
		return std::static_pointer_cast<Asset<T>>(asset);
	}
	template<typename T>
	inline static Array<AssetHandle> AssetManager::GetAssetHandleList()
	{
		Array<AssetHandle> assets;
		for (auto& [handle, metadata] : s_Registry)
		{
			if (metadata.Type == TypeToAssetType<T>::Value)
			{
				assets.Add(handle);
			}
		}
		return assets;
	}

	template<typename T>
	inline static Array<AssetMetadata>	AssetManager::GetAssetMetadatas()
	{
		return GetAssetMetadatasByAssetType(TypeToAssetType<T>::Value());
	}

	template<typename T>
	inline static std::shared_ptr<Asset<T>> AssetManager::RegisterAsset(T* object, const std::filesystem::path& path, bool save)
	{
		SPX_CORE_ASSERT(!std::filesystem::is_directory(path), "The given path cannot be a directory!!");

		// create metadata and asset pointer
		AssetMetadata metadata;
		metadata.Handle = AssetHandle::Generate();
		metadata.Path = path;
		metadata.Path.replace_extension(ASSET_EXTENSION);
		metadata.Type = TypeToAssetType<T>::Value();

		std::shared_ptr<Asset<T>> asset = std::make_shared<Asset<T>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = object;

		// make asset manager manage the asset and save data into .spxasset file
		ManageAsset(asset, metadata);
		AssetMetadata& metadataRef = s_Registry[metadata.Handle];
		AssetDependencySolver<T>()(*object, metadataRef);
		SubAssetSolver<T>()(*object, metadataRef);

		if (save)
		{
			AssetImporter::Save(metadataRef);
		}

		return asset;
	}

}

#define RegisterAssetType(Type) RegisterAssetType(#Type)
#define RegisterAssetTypeExtensions(Type, Extensions) RegisterAssetTypeExtensions(#Type, Extensions)
#define IsAssetRegistered(Type) IsAssetRegistered(#Type)
