#pragma once

#include "Core/Core.h"
#include "Serialization/Reflection/ReflectionSerializer.h"


namespace Sphynx
{
	class SPHYNX_API ActorSerializer
	{
	public:
		ActorSerializer(const class Actor& actor, class Writer&& writer);
		ActorSerializer(const class Actor& actor, const Reflection::Class& cClass, class Writer&& writer);

		void Serialize();

	private:
		void SerializeComponent(const Reflection::Class& componentClass);

	private:
		const Actor& m_Actor;
		const Reflection::Type& m_Type;
		Writer& m_Writer;
	};
}