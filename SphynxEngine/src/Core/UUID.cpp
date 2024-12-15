#include "spxpch.h"
#include "UUID.h"

#include <uuid.h>


namespace Sphynx
{
	namespace
	{
		static std::random_device s_RandomDevice;
		static std::mt19937_64 s_RandomEngine(s_RandomDevice());
		static uuids::basic_uuid_random_generator<std::mt19937_64> s_Generator(s_RandomEngine);

		UUID FromSTDUUIDToSphynxUUID(const uuids::uuid& stduuid)
		{
			SPX_CORE_ASSERT(sizeof(uuids::uuid) == sizeof(UUID), "UUID sizes should be equals!");

			UUID uuid;
			memcpy(&uuid, &stduuid, sizeof(stduuid));
			return uuid;
		}
	}

	const UUID UUID::Invalid{};

	UUID UUID::Generate()
	{
		return FromSTDUUIDToSphynxUUID(s_Generator());
	}

	UUID UUID::FromString(const std::string& str)
	{
		auto uuid = uuids::uuid::from_string(str);
		return uuid.has_value() ? FromSTDUUIDToSphynxUUID(uuid.value()) : UUID::Invalid;
	}

	std::string UUID::ToString(const UUID& uuid)
	{
		return uuids::to_string(uuid.m_Data);
	}

	bool operator==(const UUID& lhs, const UUID& rhs)
	{
		return lhs.m_Data == rhs.m_Data;
	}

	bool operator!=(const UUID& lhs, const UUID& rhs)
	{
		return lhs.m_Data != rhs.m_Data;
	}

	bool operator<(const UUID& lhs, const UUID& rhs)
	{
		return lhs.m_Data < rhs.m_Data;
	}
}



#include "Serialization/Serialization.h"
#include "Serialization/Writer.h"
#include "Serialization/Reader.h"
#include "Serialization/YAML/YAMLWriter.h"
#include "Serialization/YAML/YAMLReader.h"


template<> void Sphynx::Serialization::Write<>(Sphynx::Writer& writer, const UUID& uuid)
{
	writer.Write(UUID::ToString(uuid));
}

template<> void Sphynx::Serialization::Read<>(const Sphynx::Reader& reader, UUID& uuid)
{
	std::string str; reader.Read(str);
	uuid = UUID::FromString(str);
}

template<> void Sphynx::Serialization::Write<>(Sphynx::YAMLWriter& writer, const UUID& uuid)
{
	writer.Write(UUID::ToString(uuid));
}

template<> void Sphynx::Serialization::Read<>(const Sphynx::YAMLReader& reader, UUID& uuid)
{
	std::string str; reader.Read(str);
	uuid = UUID::FromString(str);
}
