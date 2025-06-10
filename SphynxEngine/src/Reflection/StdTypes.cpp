#include "StdTypes.h"
#include "Reflection.h"
#include <charconv>
#include <cwchar>


#define INSTANTIATE(_Type) template SPHYNX_API const ::Sphynx::Reflection::Type& GetType<_Type>();
#define X(_Type) \
namespace details { \
template<> struct SPHYNX_API ClassImplWrapper<_Type> { \
		inline static TypeRegister<_Type> s_TypeRegister{}; \
}; \
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
#undef X

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

		// FromString/ToString definitions		
		bool ToString(const bool& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, value ? "true" : "false");
			return result >= 0 && result < size;
		}

		bool FromString(bool& value, const char* buffer)
		{
			if (!strcmp(buffer, "false") || !strcmp(buffer, "False"))
			{
				value = false;
				return !value;
			}
			if (!strcmp(buffer, "true") || !strcmp(buffer, "True"))
			{
				value = true;
				return value;
			}

			return false;
		}

		bool ToString(const char& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "'%c'", value);
			return result >= 0 && result < size;
		}

		bool FromString(char& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			if (size != 3) return false;
			if (buffer[0] != '\'' || buffer[2] != '\'') return false;

			value = buffer[1];
			return true;
		}
		
		bool ToString(const signed char& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%d", value);
			return result >= 0 && result < size;
		}
		
		bool FromString(signed char& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}

		bool ToString(const wchar_t& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%lc", static_cast<std::wint_t>(value));
			return result >= 0 && result < size;
		}

		bool FromString(wchar_t& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			if (size != 3) return false;
			if (buffer[0] != '\'' || buffer[2] != '\'') return false;

			value = static_cast<wchar_t>(buffer[1]);
			return true;
		}

		bool ToString(const short& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%d", value);
			return result >= 0 && result < size;
		}
		
		bool FromString(short& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}

		bool ToString(const int& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%d", value);
			return result >= 0 && result < size;
		}

		bool FromString(int& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}
		
		bool ToString(const long& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%d", value);
			return result >= 0 && result < size;
		}

		bool FromString(long& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}

		bool ToString(const long long& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%zd", value);
			return result >= 0 && result < size;
		}

		bool FromString(long long& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}

		bool ToString(const float& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%f", value);
			return result >= 0 && result < size;
		}
		
		bool FromString(float& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}
		
		bool ToString(const double& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%f", value);
			return result >= 0 && result < size;
		}
		
		bool FromString(double& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}
		
		bool ToString(const long double& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%f", value);
			return result >= 0 && result < size;
		}

		bool FromString(long double& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}
		
		bool ToString(const unsigned char& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%u", value);
			return result >= 0 && result < size;
		}
		
		bool FromString(unsigned char& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}
		
		bool ToString(const unsigned short& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%u", value);
			return result >= 0 && result < size;
		}
		bool FromString(unsigned short& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}

		bool ToString(const unsigned int& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%u", value);
			return result >= 0 && result < size;
		}
		
		bool FromString(unsigned int& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}
		
		bool ToString(const unsigned long& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%u", value);
			return result >= 0 && result < size;
		}
		
		bool FromString(unsigned long& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}
		
		bool ToString(const unsigned long long& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "%zu", value);
			return result >= 0 && result < size;
		}
		
		bool FromString(unsigned long long& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			const auto result = std::from_chars(buffer, buffer + size, value);
			return result.ec == std::errc{} && result.ptr == buffer + size;
		}

		bool ToString(const ::std::string& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "\"%s\"", value.data());
			return result >= 0 && result < size;
		}
		
		bool FromString(::std::string& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			if (size < 3) return false;
			if (buffer[0] != '\"' || buffer[size - 1] != '\"') return false;

			value.resize(size - 2);
			return std::strncpy(value.data(), buffer + 1, size - 2) == value.data();
		}
		
		bool ToString(const ::std::wstring& value, char* buffer, size_t size)
		{
			const int result = std::snprintf(buffer, size, "\"%ls\"", value.c_str());
			return result >= 0 && result < size;
		}
		
		bool FromString(::std::wstring& value, const char* buffer)
		{
			const size_t size = std::strlen(buffer);
			if (size < 3) return false;
			if (buffer[0] != '\"' || buffer[size - 1] != '\"') return false;

			value.resize(size - 2);
			const int result = std::mbstowcs(value.data(), buffer + 1, size - 2);
			return result >= 0 && result < size;
		}
		
		bool ToString(const ::std::filesystem::path& value, char* buffer, size_t size)
		{
			using string_type = typename ::std::filesystem::path::string_type;
			const string_type& str = value.native();
			return ToString(str, buffer, size);
		}
		
		bool FromString(::std::filesystem::path& value, const char* buffer)
		{
			using string_type = typename ::std::filesystem::path::string_type;
			string_type str{};
			const bool result = FromString(str, buffer);
			if (result)
			{
				value = str;
			}

			return result;
		}
	}
}
