#include "spxpch.h"
#include "SceneAssetImporter.h"
#include "Asset/AssetManager.h"
#include "Asset/AssetMetadata.h"
#include "SceneAsset.h"

#include "Serialization/Scene/SceneSerializer.h"
#include "Serialization/Scene/SceneDeserializer.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> SceneAssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		SPX_CORE_LOG_TRACE("Importing scene {}", metadata.Path.string().c_str());
		return Load(metadata);
	}

	std::shared_ptr<IAsset> SceneAssetImporter::Load(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Loading scene {}", metadata.Path.string().c_str());

		Scene* scene = new Scene();
		YAMLReader reader{ metadata.Path };
		SceneDeserializer deserializer{ *scene, reader };

		std::shared_ptr<Asset<Scene>> asset = std::make_shared<Asset<Scene>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = scene;

		return asset;
	}

	void SceneAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving scene {}", metadata.Path.string().c_str());

		std::shared_ptr<Asset<Scene>> sceneAsset = AssetManager::GetAsset<Scene>(metadata.Handle);

		YAMLWriter writer{ metadata.Path };
		SceneSerializer serializer{ *sceneAsset->Asset, writer };
	}
}
