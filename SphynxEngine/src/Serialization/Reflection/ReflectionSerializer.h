#pragma once

#include "Core/Core.h"
#include "Reflection/Class.h"


namespace Sphynx
{
	class SPHYNX_API ReflectionSerializer
	{
	public:
		template<typename T>
		ReflectionSerializer(const T& obj, class Writer& writer);

		void Serialize();

	private:
		const void* m_Obj;
		Writer& m_Writer;
		const Reflection::Class& m_Class;
	};


	template<typename T>
	inline ReflectionSerializer::ReflectionSerializer(const T& obj, Writer& writer) :
		m_Obj(obj), m_Writer(writer), m_Class(GetClass<T>())
	{
	}
}