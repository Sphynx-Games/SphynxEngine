#pragma once

#include "Core/Core.h"
#include "Reflection/Reflection.h"


namespace Sphynx
{
	class SPHYNX_API ReflectionDeserializer
	{
	public:
		template<typename T>
		ReflectionDeserializer(const T& obj, class Reader& reader) :
			m_Obj(&obj),
			m_Type(Reflection::GetType<T>()),
			m_Reader(reader)
		{
		}

		ReflectionDeserializer(const void* obj, const Reflection::Type& type, class Reader& reader);

	public:
		void Deserialize();

	private:
		const void* m_Obj;
		const Reflection::Type& m_Type;
		Reader& m_Reader;

	};
}