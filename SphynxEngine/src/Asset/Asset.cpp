#include "Asset.h"


#include "Serialization/Serialization.h"
#include "Serialization/Writer.h"
#include "Serialization/Reader.h"
#include "Serialization/YAML/YAMLWriter.h"
#include "Serialization/YAML/YAMLReader.h"


namespace Sphynx
{
	bool AssetType::operator==(const AssetType& other) const
	{
		return Type == other.Type;
	}

	bool AssetType::operator<(const AssetType& other) const
	{
		return Type < other.Type;
	}
}

template<> SPHYNX_API void Sphynx::Serialization::Write<>(Sphynx::Writer& writer, const AssetType& assetType)
{
	SPX_CORE_ASSERT(assetType.Type != nullptr, "AssetType is invalid");
	writer.Write(assetType.Type->Name);
}

template<> SPHYNX_API void Sphynx::Serialization::Read<>(const Sphynx::Reader& reader, AssetType& assetType)
{
	std::string str; reader.Read(str);
	assetType.Type = Sphynx::Reflection::Registry::TryGetType(str.c_str());
}

template<> SPHYNX_API void Sphynx::Serialization::Write<>(Sphynx::YAMLWriter& writer, const AssetType& assetType)
{
	SPX_CORE_ASSERT(assetType.Type != nullptr, "AssetType is invalid");
	writer.Write(assetType.Type->Name);
}

template<> SPHYNX_API void Sphynx::Serialization::Read<>(const Sphynx::YAMLReader& reader, AssetType& assetType)
{
	std::string str; reader.Read(str);
	assetType.Type = Sphynx::Reflection::Registry::TryGetType(str.c_str());
}
