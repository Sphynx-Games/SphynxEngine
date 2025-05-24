#include "StdTypes.h"
#include "Reflection.h"

#define INSTANTIATE(_Type) template SPHYNX_API const ::Sphynx::Reflection::Type& GetType<_Type>();
#define X(_Type) \
namespace details { \
SPHYNX_API const ::Sphynx::Reflection::Type& GetTypeImpl(Tag<_Type>) \
{ \
	static const ::Sphynx::Reflection::Type type{ \
		::Sphynx::Reflection::details::Tag<_Type>{}, \
		#_Type, \
		sizeof(_Type), \
		alignof(_Type), \
		::Sphynx::Reflection::TypeKind::PRIMITIVE \
	}; \
	return type; \
} }\
INSTANTIATE(_Type)

namespace Sphynx
{
	namespace Reflection
	{
		TYPES()

		namespace details
		{
			SPHYNX_API const Type& GetTypeImpl(Tag<void>)
			{
				static const Type type{ Tag<void>{}, "void", 0, 0, TypeKind::PRIMITIVE };
				return type;
			}

			SPHYNX_API const Sphynx::Reflection::Type& GetTypeImpl(Tag<::std::string>)
			{
				static const Type type{ Tag<::std::string>{}, "std::string", sizeof(::std::string), alignof(::std::string), TypeKind::PRIMITIVE };
				return type;
			}

			SPHYNX_API const Sphynx::Reflection::Type& GetTypeImpl(Tag<::std::wstring>)
			{
				static const Type type{ Tag<::std::wstring>{}, "std::wstring", sizeof(::std::wstring), alignof(::std::wstring), TypeKind::PRIMITIVE };
				return type;
			}

			SPHYNX_API const Sphynx::Reflection::Type& GetTypeImpl(Tag<::std::filesystem::path>)
			{
				static const Type type{ Tag<::std::filesystem::path>{}, "std::filesystem::path", sizeof(::std::filesystem::path), alignof(::std::filesystem::path), TypeKind::PRIMITIVE };
				return type;
			}
		}

		INSTANTIATE(void)
		INSTANTIATE(::std::string)
		INSTANTIATE(::std::wstring)
		INSTANTIATE(::std::filesystem::path)

	}
}
