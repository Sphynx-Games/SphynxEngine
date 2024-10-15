#pragma once

#include "Core/Core.h"
#include "Reflection/Reflection.h"


namespace Sphynx
{
	class SPHYNX_API ReflectionSerializer
	{
	public:
		template<typename T>
		ReflectionSerializer(const T& obj, class Writer& writer) :
			m_Obj(&obj),
			m_Type(Reflection::GetType<T>()),
			m_Writer(writer)
		{
		}

		ReflectionSerializer(const void* obj, const Reflection::Type& type, class Writer& writer);

	public:
		void Serialize();

	private:
		const void* m_Obj;
		const Reflection::Type& m_Type;
		Writer& m_Writer;

	};
}