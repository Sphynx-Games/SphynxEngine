#include "spxpch.h"
#include "AssetManager.h"
#include "Logging/Log.h"

#include "Asset/Texture/TextureAssetImporter.h"

#undef RegisterAssetType
#undef RegisterAssetTypeExtensions
#undef IsAssetRegistered
#define TYPE_TO_ASSETTYPE(Type) #Type
#define REGISTER_ASSETTYPE(Type, Importer, Extension, ...) \
{\
	RegisterAssetType(TYPE_TO_ASSETTYPE(Type));\
	RegisterAssetTypeExtensions(TYPE_TO_ASSETTYPE(Type), { Extension, ##__VA_ARGS__ });\
	AssetImporter::RegisterImporter(TYPE_TO_ASSETTYPE(Type), Importer);\
}

namespace Sphynx
{
	namespace
	{
		static std::unordered_map<AssetHandle, std::shared_ptr<IAsset>> s_LoadedAssets;
		static std::unordered_map<std::string, AssetType> s_AssetTypeExtensions;
	}

	AssetTypeRegistry AssetManager::s_TypeRegistry = {};
	AssetRegistry AssetManager::s_Registry = {};

	void AssetManager::Init()
	{
		SPX_LOG_CORE_TRACE("Initializing AssetManager");

		// Register all engine related assets here,
		// custom assets will be registered elsewhere

		// Register a "Invalid" type as a utility value
		REGISTER_ASSETTYPE(Invalid, nullptr, "");

		// Register Texture
		REGISTER_ASSETTYPE(Texture, &TextureAssetImporter::Import, ".png", ".jpg", "jpeg"); // NOTE: add more extensions if needed

		// TODO: load all managed assets (needs some kind of project asset registry file)
	}

	void AssetManager::Shutdown()
	{
		SPX_LOG_CORE_TRACE("Shutting down AssetManager");

		// TODO: release all managed assets
	}

	void AssetManager::RegisterAssetType(const AssetType& assetType)
	{
		SPX_CORE_ASSERT(!IsAssetTypeRegistered(assetType), "Asset type \"{}\" is already registered!", assetType);
		SPX_LOG_CORE_TRACE("Asset type registered: {}", assetType);
		s_TypeRegistry.insert(assetType);
	}

	void AssetManager::RegisterAssetTypeExtensions(const AssetType& assetType, const std::initializer_list<std::string>& extensions)
	{
		SPX_CORE_ASSERT(IsAssetTypeRegistered(assetType), "Asset type \"{}\" is not registered!", assetType);
		for (const std::string& extension : extensions)
		{
			SPX_CORE_ASSERT(s_AssetTypeExtensions.find(extension) == s_AssetTypeExtensions.end(), "Extension alerady in use while registering a {}!", assetType);
			s_AssetTypeExtensions[extension] = assetType;
		}
	}

	bool AssetManager::IsAssetTypeRegistered(const AssetType& assetType)
	{
		return s_TypeRegistry.find(assetType) != s_TypeRegistry.end();
	}

	std::shared_ptr<IAsset> AssetManager::Import(const std::filesystem::path& path)
	{
		// create metadata
		AssetMetadata metadata;
		metadata.Handle = AssetHandle::Generate();
		metadata.Path = path;
		metadata.Type = GetAssetTypeFromExtension(path.extension().string());

		// import asset normally
		std::shared_ptr<IAsset> asset = AssetImporter::Import(metadata);
		if (asset != nullptr)
		{
			asset->Handle = metadata.Handle;
			s_LoadedAssets[metadata.Handle] = asset;
			s_Registry[metadata.Handle] = metadata;

			// TODO: serialize into disc (asset registry file)
		}

		return asset;
	}

	std::shared_ptr<IAsset> AssetManager::GetAsset(AssetHandle handle)
	{
		// -----------------------------------------------------------
		// NOTE: for now it is assumed the assets are not packed
		// -----------------------------------------------------------
		
		// Check if the handle is valid
		if (handle == AssetHandle::Invalid)
		{
			SPX_LOG_CORE_ERROR("Trying to get an asset with an invalid handle!");
			return nullptr;
		}

		// Check if the asset is loaded, if not load it
		if (!IsAssetLoaded(handle))
		{
			s_LoadedAssets[handle] = AssetImporter::Import(GetMetadata(handle));
		}

		return s_LoadedAssets[handle];
	}

	const AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		SPX_CORE_ASSERT(IsAssetLoaded(handle), "Asset \"{}\" is not loaded!");
		return s_Registry[handle];
	}

	bool AssetManager::IsAssetLoaded(AssetHandle handle)
	{
		return s_LoadedAssets.find(handle) != s_LoadedAssets.end();
	}

	AssetType AssetManager::GetAssetType(AssetHandle handle)
	{
		return GetMetadata(handle).Type;
	}

	AssetType AssetManager::GetAssetTypeFromExtension(const std::string& extension)
	{
		if (s_AssetTypeExtensions.find(extension) == s_AssetTypeExtensions.end())
		{
			return TYPE_TO_ASSETTYPE(Invalid);
		}

		return s_AssetTypeExtensions[extension];
	}

}
