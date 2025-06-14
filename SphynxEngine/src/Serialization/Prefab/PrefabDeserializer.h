#pragma once

#include "Core/Core.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"


namespace Sphynx
{
	class SPHYNX_API PrefabDeserializer
	{
	public:
		PrefabDeserializer(class Prefab& prefab, Reader&& reader);

		void Deserialize();

	private:
		void DeserializeComponent(const Reflection::Class& componentClass);

	private:
		Prefab& m_Prefab;
		const Reader& m_Reader;
	};
}