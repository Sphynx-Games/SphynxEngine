#pragma once

#include "Core/Core.h"
#include "Traits/Traits.h"

#include <cstring>


namespace Sphynx
{
	/*template<typename T>
	void Serialize(class Writer&, const T&);*/

	class SPHYNX_API Writer
	{
	public:
		// iterable types
		template<typename TIterable>
		typename std::enable_if<Traits::is_iterable<TIterable>::value>::type
		Write(const TIterable& iterable)
		{
			size_t size = std::distance(std::begin(iterable), std::end(iterable));
			Write((const void*)&size, sizeof(size));

			for (const auto& elem : iterable)
			{
				Write(elem);
			}
		}

		// normal types (eg. builtin types and user-defined classes/structs)
		template<typename T>
		typename std::enable_if<!Traits::is_iterable<T>::value>::type
		Write(const T& t)
		{
			if constexpr (Traits::is_tuple_like<T>::value)
			{
				using Sequence = std::make_index_sequence<std::tuple_size<T>::value>;
				foreach_element_in_tuple(
					[&](const auto& item) { Write(item); },
					t,
					Sequence{});
			}
			else if constexpr (std::is_trivial_v<T>)
			{
				Write((const void*)&t, sizeof(T));
			}
			/*else
			{
				Serialize<T>(*this, t);
			}*/
		}

		void Write(const void* data, size_t size)
		{
			WriteImpl(data, size);
		}

		void Write(const char* str)
		{
			size_t size = strlen(str);
			Write(size);
			Write(str, size + 1);
		}

		void Write(const wchar_t* str)
		{
			size_t size = wcslen(str);
			Write(size);
			Write(str, size + 1);
		}

		void Write(const std::string& str)
		{
			size_t size = str.size();
			Write(size);
			Write(str.data(), (size + 1) * sizeof(typename std::string::value_type));
		}

		void Write(const std::wstring& str)
		{
			size_t size = str.size();
			Write(size);
			Write(str.data(), (size + 1) * sizeof(typename std::wstring::value_type));
		}

		void Write(const std::filesystem::path& path)
		{
			Write(path.wstring());
		}

	protected:
		virtual void WriteImpl(const void* data, size_t size) = 0;
	};
}