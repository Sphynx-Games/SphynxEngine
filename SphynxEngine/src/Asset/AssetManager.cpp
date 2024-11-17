#include "spxpch.h"
#include "AssetManager.h"
#include "Logging/Log.h"
#include "Asset/Texture/TextureAssetImporter.h"
#include "Asset/Sprite/SpriteAssetImporter.h"
#include "Asset/Spritesheet/SpritesheetAssetImporter.h"
#include "Asset/Font/FontAssetImporter.h"
#include "Serialization/FileReader.h"
#include "Serialization/FileWriter.h"


#undef RegisterAssetType
#undef RegisterAssetTypeExtensions
#undef IsAssetRegistered
#define REGISTER_ASSETTYPE(Type, Importer, ...) \
{\
	RegisterAssetType(TYPE_TO_ASSETTYPE(Type));\
	RegisterAssetTypeExtensions(TYPE_TO_ASSETTYPE(Type), { ##__VA_ARGS__ });\
	AssetImporter::RegisterImporter(TYPE_TO_ASSETTYPE(Type), &Importer::Import);\
	AssetImporter::RegisterLoader(TYPE_TO_ASSETTYPE(Type), &Importer::Load);\
	AssetImporter::RegisterSaver(TYPE_TO_ASSETTYPE(Type), &Importer::Save);\
}
#define REGISTER_INVALID_ASSETTYPE(Type, Importer, ...) \
{\
	RegisterAssetType(TYPE_TO_ASSETTYPE(Type));\
	RegisterAssetTypeExtensions(TYPE_TO_ASSETTYPE(Type), { ##__VA_ARGS__ });\
	AssetImporter::RegisterImporter(TYPE_TO_ASSETTYPE(Type), nullptr);\
	AssetImporter::RegisterLoader(TYPE_TO_ASSETTYPE(Type), nullptr);\
	AssetImporter::RegisterSaver(TYPE_TO_ASSETTYPE(Type), nullptr);\
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

	static const std::filesystem::path ASSET_REGISTRY_FILEPATH = std::filesystem::path("Assets\\assetRegistry").replace_extension(ASSET_EXTENSION);
	

	void AssetManager::Init()
	{
		SPX_CORE_LOG_TRACE("Initializing AssetManager");

		// Register all engine related assets here,
		// custom assets will be registered elsewhere

		// Register a "Invalid" type as a utility value
		REGISTER_INVALID_ASSETTYPE(Invalid, nullptr, "");

		// Register Asset types (Texture, Sprite...)
		REGISTER_ASSETTYPE(Texture, TextureAssetImporter, ".png", ".jpg", ".jpeg"); // NOTE: add more extensions if needed
		REGISTER_ASSETTYPE(Spritesheet, SpritesheetAssetImporter);
		REGISTER_ASSETTYPE(Sprite, SpriteAssetImporter);
		REGISTER_ASSETTYPE(Font, FontAssetImporter, ".ttf");

		// load all managed assets into the asset registry
		FileReader reader(ASSET_REGISTRY_FILEPATH);

		if (reader.IsValid())
		{
			size_t numAssets;
			reader.Read(numAssets);
			for (size_t i = 0; i < numAssets; ++i)
			{
				AssetMetadata metadata;
				reader.Read(metadata.Handle);
				reader.Read(metadata.Type);
				reader.Read(metadata.Path);

				// dependencies
				size_t num;
				reader.Read(num);
				for (size_t j = 0; j < num; ++j)
				{
					AssetHandle dependencyHandle;
					reader.Read(dependencyHandle);
					metadata.Dependencies.Add(dependencyHandle);
				}

				s_Registry.Add(metadata.Handle, metadata);
			}
		}		
	}

	void AssetManager::Shutdown()
	{
		SPX_CORE_LOG_TRACE("Shutting down AssetManager");

		// write all managed assets into the asset registry file
		FileWriter writer(ASSET_REGISTRY_FILEPATH);

		writer.Write(s_Registry.GetKeys().Size());
		for (const auto& [handle, metadata] : s_Registry)
		{
			writer.Write(handle);
			writer.Write(metadata.Type);
			writer.Write(metadata.Path);

			writer.Write(metadata.Dependencies.Size());
			for (AssetHandle dependencyHandle : metadata.Dependencies)
			{
				writer.Write(dependencyHandle);
			}
		}

		s_LoadedAssets.clear();
	}

	void AssetManager::RegisterAssetType(const AssetType& assetType)
	{
		SPX_CORE_ASSERT(!IsAssetTypeRegistered(assetType), "Asset type \"{}\" is already registered!", assetType);
		SPX_CORE_LOG_TRACE("Asset type registered: {}", assetType);
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
			s_LoadedAssets[handle] = AssetImporter::Load(GetMetadata(handle));
		}

		return s_LoadedAssets[handle];
	}

	const AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		SPX_CORE_ASSERT(s_Registry.ContainsKey(handle), "Asset Registry does not contain the asset with handle \"{}\"!", AssetHandle::ToString(handle));
		return s_Registry[handle];
	}

	const AssetMetadata& AssetManager::GetMetadataFromPath(const std::filesystem::path& path)
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
