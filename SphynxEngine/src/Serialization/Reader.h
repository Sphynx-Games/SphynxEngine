#pragma once

#include "Core/Core.h"
#include "Traits/Traits.h"

#include <fstream>
#include <iostream>
#include <cstring>


namespace Sphynx
{
	/*template<typename T>
	void Deserialize(const class Reader&, T& obj);*/

	class SPHYNX_API Reader
	{
	public:
		// iterable types
		template<typename TIterable>
		typename std::enable_if<Traits::is_iterable<TIterable>::value>::type
		Read(TIterable& iterable)
		{
			size_t size = 0;
			Read((void*)&size, sizeof(size));
			std::cout << "Read size: " << size << std::endl;

			if constexpr (has_reserve<TIterable>::value)
			{
				iterable.reserve(size);
			}

			// TODO: emplace or push_back or whatever
			using T = typename TIterable::value_type;
			for (size_t i = 0; i < size; ++i)
			{
				if constexpr (has_insert<TIterable>::value)
				{
					T t; Read(t);
					iterable.insert(std::end(iterable), t);
				}
				else
				{
					T& t = iterable[i];
					Read(t);
				}
			}
		}

		// normal types (eg. builtin types and user-defined classes/structs)
		template<typename T>
		typename std::enable_if<!Traits::is_iterable<T>::value>::type
		Read(T& t)
		{
			if constexpr (Traits::is_tuple_like<T>::value)
			{
				using Sequence = std::make_index_sequence<std::tuple_size<T>::value>;
				foreach_element_in_tuple(
					[&](auto& item) { Read(item); },
					t,
					Sequence{});
			}
			else if constexpr (std::is_trivial_v<T>)
			{
				Read((void*)&t, sizeof(T));
			}
			/*else
			{
				Deserialize<T>(*this, t);
			}*/
		}

		void Read(void* data, size_t size)
		{
			ReadImpl(data, size);
		}

		void Read(char* str)
		{
			size_t i = 0;
			char c = '\0';
			do
			{
				Read(&c, sizeof(char));
				str[i] = c;
				++i;
			} while (c != '\0');
		}

		template<>
		void Read<std::string>(std::string& str)
		{
			char c = '\0';
			str = "";
			do
			{
				Read(&c, sizeof(char));
				str += c;
			} while (c != '\0');

			std::cout << "      - Value: " << str << std::endl;
		}
		
	protected:
		virtual void ReadImpl(void* data, size_t size) = 0;
	};
}