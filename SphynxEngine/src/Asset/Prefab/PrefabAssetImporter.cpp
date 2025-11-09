#include "spxpch.h"
#include "PrefabAssetImporter.h"

#include "Asset/AssetManager.h"
#include "PrefabAsset.h"

#include "Serialization/Actor/ActorSerializer.h"
#include "Serialization/Actor/ActorDeserializer.h"
#include "Component/NameComponent.h"


namespace Sphynx
{
	std::shared_ptr<IAsset> PrefabAssetImporter::Import(const AssetMetadata& metadata, const std::filesystem::path& path)
	{
		SPX_CORE_LOG_TRACE("Importing prefab {}", metadata.Path.string().c_str());
		return Load(metadata);
	}

	std::shared_ptr<IAsset> PrefabAssetImporter::Load(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Loading prefab {}", metadata.Path.string().c_str());

		Prefab* prefab = new Prefab();
		prefab->AddComponent<NameComponent>();
		YAMLReader reader{ metadata.Path };
		ActorDeserializer deserializer{ *prefab, Reflection::GetClass<Prefab>(), reader };
		deserializer.Deserialize();

		std::shared_ptr<Asset<Prefab>> asset = std::make_shared<Asset<Prefab>>();
		asset->Handle = metadata.Handle;
		asset->Type = metadata.Type;
		asset->Asset = prefab;

		return asset;
	}

	void PrefabAssetImporter::Save(const AssetMetadata& metadata)
	{
		SPX_CORE_LOG_TRACE("Saving prefab {}", metadata.Path.string().c_str());

		std::shared_ptr<Asset<Prefab>> prefabAsset = AssetManager::GetAsset<Prefab>(metadata.Handle);

		YAMLWriter writer{ metadata.Path };
		ActorSerializer serializer{ *prefabAsset->Asset, Reflection::GetClass<Prefab>(), writer };
		serializer.Serialize();
	}
}
