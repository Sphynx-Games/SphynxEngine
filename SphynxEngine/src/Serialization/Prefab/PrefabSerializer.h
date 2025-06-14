#pragma once

#include "Core/Core.h"
#include "Serialization/Reflection/ReflectionSerializer.h"


namespace Sphynx
{
	class SPHYNX_API PrefabSerializer
	{
	public:
		PrefabSerializer(const class Prefab& prefab, class Writer&& writer);

		void Serialize();

	private:
		void SerializeComponent(const Reflection::Class& componentClass);

	private:
		const Prefab& m_Prefab;
		Writer& m_Writer;
	};
}