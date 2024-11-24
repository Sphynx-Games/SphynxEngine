#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Asset.h"
#include "AssetMetadata.h"
#include "Container/Map.h"
#include "AssetMetadata.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"
#include "Serialization/Reflection/ReflectionSerializer.h"
#include "Serialization/FileReader.h"
#include "Serialization/FileWriter.h"

#include "Serialization/YAML/YAMLWriter.h"


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

		static AssetMetadataHeader DeserializeAssetHeader(const Reader& reader)
		{
			AssetMetadataHeader header;
			ReflectionDeserializer headerDeserializer(header, reader);
			headerDeserializer.Deserialize();

			SPX_CORE_ASSERT(header.SphynxAsset == SPHYNX_ASSET_HEADER, "The file is not recognised as an Sphynx Asset!!");

			return header;
		}

		template<typename T>
		static T DeserializeAssetMetadata(const Reader& reader)
		{
			T specificMetadata;
			ReflectionDeserializer deserializer(specificMetadata, reader);
			deserializer.Deserialize();

			return specificMetadata;
		}

		template<typename T>
		static T DeserializeAsset(const AssetMetadata& metadata)
		{
			FileReader reader(metadata.Path);
			SPX_CORE_ASSERT(reader.IsValid(), "Could not open file: {} !!", metadata.Path);

			AssetMetadataHeader header = DeserializeAssetHeader(reader);

			SPX_CORE_ASSERT(header.Type == metadata.Type, "Asset must be of type \"{}\", but is of type \"{}\"!!", metadata.Type, header.Type);

			return DeserializeAssetMetadata<T>(reader);
		}

		template<typename T>
		static void SerializeAsset(const AssetMetadata& metadata, const T& specificMetadata)
		{
			YAMLWriter writer(metadata.Path);
			SPX_CORE_ASSERT(writer.IsValid(), "Could not open file: {} !!", metadata.Path);

			AssetMetadataHeader header;
			header.SphynxAsset = SPHYNX_ASSET_HEADER;
			header.Type = metadata.Type;

			ReflectionSerializer::Serialize(header, writer);
			ReflectionSerializer::Serialize(specificMetadata, writer);
		}

	private:
		static HashMap<AssetType, AssetImportFunction> s_AssetImportFunctions;
		static HashMap<AssetType, AssetLoadFunction> s_AssetLoadFunctions;
		static HashMap<AssetType, AssetSaveFunction> s_AssetSaveFunctions;

	};
}
