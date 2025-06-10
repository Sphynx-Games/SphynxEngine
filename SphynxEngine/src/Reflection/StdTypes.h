#pragma once

#include "Core/Core.h"
#include "Macros.h"
#include "Tag.h"
#include "Type.h"

#include <string>
#include <filesystem>


#define TYPES() \
	X(bool)					\
	X(char)					\
	X(signed char)			\
	X(wchar_t)				\
	X(short)				\
	X(int)					\
	X(long)					\
	X(long long)			\
	X(float)				\
	X(double)				\
	X(long double)			\
	X(unsigned char)		\
	X(unsigned short)		\
	X(unsigned int)			\
	X(unsigned long)		\
	X(unsigned long long)

#define SPECIAL_TYPES()		\
	X(::std::string)		\
	X(::std::wstring)		\
	X(::std::filesystem::path)


namespace Sphynx
{
	namespace Reflection
	{
		struct Type;

		namespace details
		{
			template<typename T> struct Tag;
		}

		template<typename T> const Type& GetType();
		#define X(_Type) namespace details { extern SPHYNX_API const ::Sphynx::Reflection::Type& GetTypeImpl(Tag<_Type>); } \
			extern template SPHYNX_API const ::Sphynx::Reflection::Type& GetType<_Type>();

		TYPES()
		SPECIAL_TYPES()
		X(void)

		#undef X
		
		// ToString/FromString declarations
		#define X(_Type)	bool ToString(const _Type& value, char* buffer, size_t size); \
							bool FromString(_Type& value, const char* buffer);
		TYPES()
		SPECIAL_TYPES()
		#undef X
	}
}
