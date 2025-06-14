#include "spxpch.h"
#include "AssetManager.h"

#include "Asset/Invalid/InvalidAsset.h"

#include "Renderer/Texture.h"
#include "Asset/Texture/TextureAssetImporter.h"

#include "Renderer/Font.h"
#include "Asset/Font/FontAssetImporter.h"

#include "Scene/Scene.h"
#include "Asset/Scene/SceneAssetImporter.h"

#include "Renderer/Sprite.h"
#include "Asset/Sprite/SpriteAssetImporter.h"
#include "Asset/Spritesheet/SpritesheetAssetImporter.h"

#include "Asset/Prefab/PrefabAsset.h"
#include "Asset/Prefab/PrefabAssetImporter.h"

#include "Serialization/YAML/YAMLWriter.h"
#include "Serialization/YAML/YAMLReader.h"


#undef RegisterAssetType
#undef RegisterAssetTypeExtensions
#undef IsAssetRegistered
#define REGISTER_ASSETTYPE(Type, Importer, ...) \
{ \
	RegisterAssetType(TYPE_TO_ASSETTYPE(Type)); \
	RegisterAssetTypeExtensions(TYPE_TO_ASSETTYPE(Type), { ##__VA_ARGS__ }); \
	AssetImporter::RegisterImporter(TYPE_TO_ASSETTYPE(Type), &Importer::Import); \
	AssetImporter::RegisterLoader(TYPE_TO_ASSETTYPE(Type), &Importer::Load); \
	AssetImporter::RegisterSaver(TYPE_TO_ASSETTYPE(Type), &Importer::Save); \
}
#define REGISTER_INVALID_ASSETTYPE(Type, Importer, ...) \
{ \
	RegisterAssetType(TYPE_TO_ASSETTYPE(Type)); \
	RegisterAssetTypeExtensions(TYPE_TO_ASSETTYPE(Type), { ##__VA_ARGS__ }); \
	AssetImporter::RegisterImporter(TYPE_TO_ASSETTYPE(Type), nullptr); \
	AssetImporter::RegisterLoader(TYPE_TO_ASSETTYPE(Type), nullptr); \
	AssetImporter::RegisterSaver(TYPE_TO_ASSETTYPE(Type), nullptr); \
}

#define UNREGISTER_ASSETTYPE(Type) \
{ \
	AssetImporter::UnregisterSaver(TYPE_TO_ASSETTYPE(Type)); \
	AssetImporter::UnregisterLoader(TYPE_TO_ASSETTYPE(Type)); \
	AssetImporter::UnregisterImporter(TYPE_TO_ASSETTYPE(Type)); \
	UnregisterAssetTypeExtensions(TYPE_TO_ASSETTYPE(Type)); \
	UnregisterAssetType(TYPE_TO_ASSETTYPE(Type)); \
}


namespace Sphynx
{
	namespace
	{
		static std::filesystem::path s_ContextPath; // absolute path
		static bool s_IsInitialized = false;
		static std::unordered_map<AssetHandle, std::shared_ptr<IAsset>> s_LoadedAssets;
		static std::unordered_map<std::string, AssetType> s_AssetTypeExtensions;
	}

	AssetTypeRegistry AssetManager::s_TypeRegistry = {};
	AssetRegistry AssetManager::s_Registry = {};

	static const std::filesystem::path ASSET_REGISTRY_FILEPATH = std::filesystem::path("Assets\\AssetRegistry" ASSET_EXTENSION);


	void AssetManager::Init()
	{
		SPX_CORE_ASSERT(!s_IsInitialized, "AssetManager is already initialized");

		SPX_CORE_LOG_TRACE("Initializing AssetManager");

		s_IsInitialized = true;
		s_ContextPath = std::filesystem::current_path();

		// Register all engine related assets here,
		// custom assets will be registered elsewhere

		// Register a "Invalid" type as a utility value
		REGISTER_INVALID_ASSETTYPE(Invalid, nullptr, "");

		// Register Asset types (Texture, Sprite...)
		REGISTER_ASSETTYPE(Texture, TextureAssetImporter, ".png", ".jpg", ".jpeg"); // NOTE: add more extensions if needed
		REGISTER_ASSETTYPE(Font, FontAssetImporter, ".ttf");

		REGISTER_ASSETTYPE(Scene, SceneAssetImporter);

		REGISTER_ASSETTYPE(Sprite, SpriteAssetImporter);
		REGISTER_ASSETTYPE(Spritesheet, SpritesheetAssetImporter);

		REGISTER_ASSETTYPE(Prefab, PrefabAssetImporter);

		// load all managed assets into the asset registry
		YAMLReader reader{ s_ContextPath / ASSET_REGISTRY_FILEPATH };
		if (reader.IsValid())
		{
			ReflectionDeserializer::Deserialize(s_Registry, reader);
		}
	}

	void AssetManager::Shutdown()
	{
		SPX_CORE_ASSERT(s_IsInitialized, "AssetManager is not initialized");

		SPX_CORE_LOG_TRACE("Shutting down AssetManager");

		// write all managed assets into the asset registry file
		YAMLWriter writer{ s_ContextPath / ASSET_REGISTRY_FILEPATH };
		ReflectionSerializer::Serialize(s_Registry, writer);

		UnloadAssets();

		// Unregister Asset types
		UNREGISTER_ASSETTYPE(Prefab);

		UNREGISTER_ASSETTYPE(Spritesheet);
		UNREGISTER_ASSETTYPE(Sprite);

		UNREGISTER_ASSETTYPE(Scene);

		UNREGISTER_ASSETTYPE(Font);
		UNREGISTER_ASSETTYPE(Texture); 

		// Unregister a "Invalid"
		UNREGISTER_ASSETTYPE(Invalid);

		s_ContextPath.clear();
		s_IsInitialized = false;
	}

	void AssetManager::RegisterAssetType(const AssetType& assetType)
	{
		SPX_CORE_ASSERT(!IsAssetTypeRegistered(assetType), "Asset type \"{}\" is already registered!", assetType);
		SPX_CORE_LOG_TRACE("Asset type registered: {}", assetType.Type->Name);
		s_TypeRegistry.insert(assetType);
	}

	void AssetManager::RegisterAssetTypeExtensions(const AssetType& assetType, const std::initializer_list<std::string>& extensions)
	{
		SPX_CORE_ASSERT(IsAssetTypeRegistered(assetType), "Asset type \"{}\" is not registered!", assetType.Type->Name);
		for (const std::string& extension : extensions)
		{
			SPX_CORE_ASSERT(s_AssetTypeExtensions.find(extension) == s_AssetTypeExtensions.end(), "Extension alerady in use while registering a {}!", assetType);
			s_AssetTypeExtensions[extension] = assetType;
		}
	}

	void AssetManager::UnregisterAssetType(const AssetType& assetType)
	{
		SPX_CORE_ASSERT(IsAssetTypeRegistered(assetType), "Asset type \"{}\" is not registered!", assetType.Type->Name);
		SPX_CORE_LOG_TRACE("Asset type unregistered: {}", assetType.Type->Name);
		s_TypeRegistry.erase(assetType);
	}

	void AssetManager::UnregisterAssetTypeExtensions(const AssetType& assetType)
	{
		SPX_CORE_ASSERT(IsAssetTypeRegistered(assetType), "Asset type \"{}\" is not registered!", assetType.Type->Name);
		
		auto isAssetType = [&](auto& data) {return data.second == assetType; };
		auto it = std::find_if(s_AssetTypeExtensions.begin(), s_AssetTypeExtensions.end(), isAssetType);
		while (it != s_AssetTypeExtensions.end())
		{
			const AssetType& type = it->second;
			if (type == assetType)
			{
				it = s_AssetTypeExtensions.erase(it);
				it = std::find_if(it, s_AssetTypeExtensions.end(), isAssetType);
			}
		}
	}

	bool AssetManager::IsAssetTypeRegistered(const AssetType& assetType)
	{
		return s_TypeRegistry.find(assetType) != s_TypeRegistry.end();
	}

	void AssetManager::AddToRegistry(const AssetMetadata& metadata)
	{
		SPX_CORE_ASSERT(!s_Registry.ContainsKey(metadata.Handle), "Handle {} already added to registry!", metadata.Handle);
		s_Registry[metadata.Handle] = metadata;
	}

	void AssetManager::RemoveFromRegistry(AssetHandle handle)
	{
		SPX_CORE_ASSERT(s_Registry.ContainsKey(handle), "Handle {} Iis not added to registry!", handle);
		s_Registry.Remove(handle);
	}

	const Sphynx::AssetRegistry& AssetManager::GetRegistry()
	{
		return s_Registry;
	}

	std::shared_ptr<IAsset> AssetManager::Import(const std::filesystem::path& path)
	{
		// create metadata
		AssetMetadata metadata;
		metadata.Handle = AssetHandle::Generate();
		metadata.Path = path;
		metadata.Path.replace_extension(ASSET_EXTENSION);
		metadata.Type = GetAssetTypeFromExtension(path.extension().string());

		// import asset normally
		std::shared_ptr<IAsset> asset = AssetImporter::Import(metadata, path);
		ManageAsset(asset, metadata);

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
			SPX_CORE_LOG_ERROR("Trying to get an asset with an invalid handle!");
			return nullptr;
		}

		// if the asset metadata is not stored in the registry, then the asset is not created yet
		if (!s_Registry.ContainsKey(handle))
		{
			return nullptr;
		}

		// Check if the asset is loaded, if not load it
		if (!IsAssetLoaded(handle))
		{
			s_LoadedAssets[handle] = AssetImporter::Load(GetAssetMetadata(handle));
		}

		return s_LoadedAssets[handle];
	}

	Sphynx::Array<Sphynx::AssetMetadata> AssetManager::GetAssetMetadatasByAssetType(const AssetType& assetType)
	{
		Array<AssetMetadata> assets;
		for (auto& [handle, metadata] : s_Registry)
		{
			if (metadata.Type == assetType)
			{
				assets.Add(metadata);
			}
		}
		return assets;
	}

	void AssetManager::UnloadAsset(AssetHandle handle)
	{
		s_LoadedAssets.erase(handle);
	}

	void AssetManager::UnloadAssets()
	{
		s_LoadedAssets.clear();
	}

	const AssetMetadata& AssetManager::GetAssetMetadata(AssetHandle handle)
	{
		SPX_CORE_ASSERT(s_Registry.ContainsKey(handle), "Asset Registry does not contain the asset with handle \"{}\"!", AssetHandle::ToString(handle));
		return s_Registry[handle];
	}

	const AssetMetadata& AssetManager::GetAssetMetadataFromPath(const std::filesystem::path& path)
	{
		auto it = std::find_if(s_Registry.begin(), s_Registry.end(), [&](const auto& registryEntry) {
			return registryEntry.second.Path == path;
			});

		if (it == s_Registry.end()) return AssetMetadata::Invalid;

		return it->second;
	}

	bool AssetManager::IsAssetLoaded(AssetHandle handle)
	{
		return s_LoadedAssets.find(handle) != s_LoadedAssets.end();
	}

	AssetType AssetManager::GetAssetType(AssetHandle handle)
	{
		return GetAssetMetadata(handle).Type;
	}

	AssetType AssetManager::GetAssetTypeFromExtension(const std::string& extension)
	{
		if (s_AssetTypeExtensions.find(extension) == s_AssetTypeExtensions.end())
		{
			return TYPE_TO_ASSETTYPE(Invalid);
		}

		return s_AssetTypeExtensions[extension];
	}

	AssetHandle AssetManager::GetAssetHandleFromAddress(void* address)
	{
		auto it = std::find_if(s_LoadedAssets.begin(), s_LoadedAssets.end(), [&](const auto& value)
			{
				return value.second->GetRawAsset() == address;
			});

		if (it != s_LoadedAssets.end())
		{
			return it->first;
		}

		return AssetHandle::Invalid;
	}

	AssetHandle AssetManager::GetAssetHandleFromPath(const std::filesystem::path& path)
	{
		for (auto& [handle, metadata] : s_Registry)
		{
			if (path == metadata.Path)
			{
				return handle;
			}
		}
		SPX_CORE_LOG_ERROR("Asset not found for required path!");
		return AssetHandle::Invalid;
	}

	void AssetManager::ManageAsset(std::shared_ptr<IAsset> asset, const AssetMetadata& metadata)
	{
		if (asset != nullptr)
		{
			asset->Handle = metadata.Handle;
			s_LoadedAssets[metadata.Handle] = asset;
			AddToRegistry(metadata);
		}
	}

}
