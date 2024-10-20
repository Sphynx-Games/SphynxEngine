#pragma once

#include "Core/Core.h"
#include "Array.h"
#include "Pair.h"
#include <unordered_map>
#include <utility>


namespace Sphynx
{
	template <typename TKey, typename TValue>
	class SPHYNX_API HashMap
	{
	public:
		using key_type = typename std::unordered_map<TKey, TValue>::key_type;
		using mapped_type = typename std::unordered_map<TKey, TValue>::mapped_type;
		using Iterator = typename std::unordered_map<TKey, TValue>::iterator;
		using ConstIterator = typename std::unordered_map<TKey, TValue>::const_iterator;

	public:
		HashMap() : m_Hashmap() {}

		HashMap(size_t initialCapacity)
		{
			m_Hashmap.reserve(initialCapacity);
		}

		HashMap(const HashMap& other) : HashMap(other.Size())
		{
			m_Hashmap = other.m_Hashmap;
		}

		HashMap(HashMap&& other)
		{
			m_Hashmap = std::move(other.m_Hashmap);
			other.RemoveAll();
		}

		~HashMap() = default;

		void Add(const TKey& key, const TValue& value)
		{
			m_Hashmap[key] = value;
		}

		void Emplace(TKey&& key, TValue&& value)
		{
			m_Hashmap.emplace(key, value);
		}

		void Remove(const TKey& key)
		{
			m_Hashmap.erase(key);
		}

		void RemoveAll()
		{
			m_Hashmap.clear();
		}

		bool ContainsKey(const TKey& key) const
		{
			return m_Hashmap.count(key);
		}

		bool ContainsValue(const TValue& value) const
		{
			for (auto& [key, val] : m_Hashmap)
			{
				if (value == val)
				{
					return true;
				}
			}
			return false;
		}

		const TValue& GetValue(const TKey& key) const
		{
			return m_Hashmap[key];
		}

		Array<TKey> GetKeys() const
		{
			Array<TKey> array = Array<TKey>(m_Hashmap.size());
			for (auto& [key, value] : m_Hashmap)
			{
				array.Add(key);
			}
			return array;
		}

		Array<TValue> GetValues()
		{
			Array<TValue> array = Array<TValue>(m_Hashmap.size());
			for (auto& [key, value] : m_Hashmap)
			{
				array.Add(value);
			}
			return array;
		}

		Array<Pair<TKey, TValue>> GetKeyValues() const
		{
			Array<Pair<TKey, TValue>> array(m_Hashmap.size());
			for (auto& [key, value] : m_Hashmap)
			{
				array.Add(Pair(key, value));
			}
			return array;
		}

		inline size_t Size() const { return m_Hashmap.size(); }
		inline bool IsEmpty() const { return m_Hashmap.empty(); }

		inline const TValue& operator [] (const TKey& key) const { return m_Hashmap.at(key); }
		inline TValue& operator [] (const TKey& key) { return m_Hashmap[key]; }

		/*HashMap& operator = (const TValue& other)
		{
			RemoveAll();
			Append(other);
			Remove

			return *this;
		}

		HashMap& operator = (TValue&& other)
		{
			RemoveAll();
			m_Array = std::move(other.m_Array);
			other.RemoveAll();
			return *this;
		}*/

		inline Iterator begin() { return m_Hashmap.begin(); }
		inline Iterator end() { return m_Hashmap.end(); }

		inline ConstIterator begin() const { return m_Hashmap.begin(); }
		inline ConstIterator end() const { return m_Hashmap.end(); }
		inline ConstIterator cbegin() const { return m_Hashmap.cbegin(); }
		inline ConstIterator cend() const { return m_Hashmap.cend(); }

	private:
		std::unordered_map<TKey, TValue> m_Hashmap;
	};
}

template<typename TKey, typename TValue> struct std::tuple_size<Sphynx::HashMap<TKey, TValue>> : std::integral_constant<size_t, 2> {};
template<typename TKey, typename TValue> struct std::tuple_element<0, Sphynx::HashMap<TKey, TValue>> { using type = TKey; };
template<typename TKey, typename TValue> struct std::tuple_element<1, Sphynx::HashMap<TKey, TValue>> { using type = TValue; };


#include "Reflection/Reflection.h"


SPX_REFLECT_TEMPLATE_CLASS_BEGIN(Sphynx::HashMap)
	SPX_REFLECT_ATTRIBUTE(AssociativeCollection)
SPX_REFLECT_TEMPLATE_CLASS_END(Sphynx::HashMap)
