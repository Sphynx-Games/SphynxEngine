#pragma once

#include "Core/Core.h"
#include "Reflection/Class.h"


namespace Sphynx
{
	class SPHYNX_API ReflectionDeserializer
	{
	public:
		template<typename T>
		ReflectionDeserializer(const T& obj, class Reader& reader);

		void Deserialize();

	private:
		const void* m_Obj;
		Reader& m_Reader;
		const Reflection::Class& m_Class;
	};


	template<typename T>
	inline ReflectionDeserializer::ReflectionDeserializer(const T& obj, Reader& reader) :
		m_Obj(&obj), m_Reader(reader), m_Class(Reflection::GetClass<T>())
	{
	}
}