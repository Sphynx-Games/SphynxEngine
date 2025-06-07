#pragma once

#include "Core/Core.h"
#include "Array.h"
#include "Pair.h"
#include <unordered_map>
#include <map>
#include <utility>


namespace Sphynx
{
	template <typename TKey, typename TValue>
	class HashMap
	{
	public:
		using key_type = typename std::unordered_map<TKey, TValue>::key_type;
		using mapped_type = typename std::unordered_map<TKey, TValue>::mapped_type;
		using Iterator = typename std::unordered_map<TKey, TValue>::iterator;
		using ConstIterator = typename std::unordered_map<TKey, TValue>::const_iterator;

	public:
		HashMap() : m_HashMap() {}

		HashMap(size_t initialCapacity)
		{
			m_HashMap.reserve(initialCapacity);
		}

		HashMap(const HashMap& other) : HashMap(other.Size())
		{
			m_HashMap = other.m_HashMap;
		}

		HashMap(HashMap&& other)
		{
			m_HashMap = std::move(other.m_HashMap);
			other.RemoveAll();
		}

		HashMap(std::initializer_list<Pair<TKey, TValue>> init) {
			for (const auto& pair : init)
			{
				m_HashMap.insert({ pair.First, pair.Second });
			}
		}

		~HashMap() = default;

		void Add(const TKey& key, const TValue& value)
		{
			m_HashMap[key] = value;
		}

		void Emplace(TKey&& key, TValue&& value)
		{
			m_HashMap.emplace(key, value);
		}

		void Remove(const TKey& key)
		{
			m_HashMap.erase(key);
		}

		void RemoveAll()
		{
			m_HashMap.clear();
		}

		bool ContainsKey(const TKey& key) const
		{
			return m_HashMap.count(key);
		}

		bool ContainsValue(const TValue& value) const
		{
			for (auto& [key, val] : m_HashMap)
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
			return m_HashMap.at(key);
		}

		Array<TKey> GetKeys() const
		{
			Array<TKey> array{ m_HashMap.size() };
			for (auto& [key, value] : m_HashMap)
			{
				array.Add(key);
			}
			return array;
		}

		Array<TValue> GetValues()
		{
			Array<TValue> array{ m_HashMap.size() };
			for (auto& [key, value] : m_HashMap)
			{
				array.Add(value);
			}
			return array;
		}

		Array<Pair<TKey, TValue>> GetKeyValues() const
		{
			Array<Pair<TKey, TValue>> array(m_HashMap.size());
			for (auto& [key, value] : m_HashMap)
			{
				array.Add(Pair(key, value));
			}
			return array;
		}

		inline size_t Size() const { return m_HashMap.size(); }
		inline bool IsEmpty() const { return m_HashMap.empty(); }

		inline HashMap& operator = (const HashMap& other) 
		{
			m_HashMap = other.m_HashMap;
			return *this;
		}
		inline HashMap& operator = (HashMap&& other)
		{
			m_HashMap = std::move(other.m_HashMap);
			return *this;
		}

		inline const TValue& operator [] (const TKey& key) const { return m_HashMap.at(key); }
		inline TValue& operator [] (const TKey& key) { return m_HashMap[key]; }

		inline Iterator begin() { return m_HashMap.begin(); }
		inline Iterator end() { return m_HashMap.end(); }

		inline ConstIterator begin() const { return m_HashMap.cbegin(); }
		inline ConstIterator end() const { return m_HashMap.cend(); }
		inline ConstIterator cbegin() const { return m_HashMap.cbegin(); }
		inline ConstIterator cend() const { return m_HashMap.cend(); }

	private:
		std::unordered_map<TKey, TValue> m_HashMap;

	};

	template <typename TKey, typename TValue>
	class SPHYNX_API Map
	{
	public:
		using key_type = typename std::map<TKey, TValue>::key_type;
		using mapped_type = typename std::map<TKey, TValue>::mapped_type;
		using Iterator = typename std::map<TKey, TValue>::iterator;
		using ConstIterator = typename std::map<TKey, TValue>::const_iterator;

	public:
		Map() : m_Map() {}

		Map(const Map& other) : Map()
		{
			m_Map = other.m_Map;
		}

		Map(Map&& other)
		{
			m_Map = std::move(other.m_Map);
			other.RemoveAll();
		}

		~Map() = default;

		void Add(const TKey& key, const TValue& value)
		{
			m_Map[key] = value;
		}

		void Emplace(TKey&& key, TValue&& value)
		{
			m_Map.emplace(key, value);
		}

		void Remove(const TKey& key)
		{
			m_Map.erase(key);
		}

		void RemoveAll()
		{
			m_Map.clear();
		}

		bool ContainsKey(const TKey& key) const
		{
			return m_Map.count(key);
		}

		bool ContainsValue(const TValue& value) const
		{
			for (auto& [key, val] : m_Map)
			{
				if (value == val)
				{
					return true;
				}
			}
			return false;
		}

		TValue& GetValue(const TKey& key)
		{
			return m_Map.at(key);
		}

		const TValue& GetValue(const TKey& key) const
		{
			return m_Map.at(key);
		}

		Array<TKey> GetKeys() const
		{
			Array<TKey> array{ m_Map.size() };
			for (auto& [key, value] : m_Map)
			{
				array.Add(key);
			}
			return array;
		}

		Array<TValue> GetValues()
		{
			Array<TValue> array{ m_Map.size() };
			for (auto& [key, value] : m_Map)
			{
				array.Add(value);
			}
			return array;
		}

		Array<Pair<TKey, TValue>> GetKeyValues() const
		{
			Array<Pair<TKey, TValue>> array(m_Map.size());
			for (auto& [key, value] : m_Map)
			{
				array.Add(Pair(key, value));
			}
			return array;
		}

		inline size_t Size() const { return m_Map.size(); }
		inline bool IsEmpty() const { return m_Map.empty(); }

		inline const TValue& operator [] (const TKey& key) const { return m_Map.at(key); }
		inline TValue& operator [] (const TKey& key) { return m_Map[key]; }

		inline Iterator begin() { return m_Map.begin(); }
		inline Iterator end() { return m_Map.end(); }

		inline ConstIterator begin() const { return m_Map.cbegin(); }
		inline ConstIterator end() const { return m_Map.cend(); }
		inline ConstIterator cbegin() const { return m_Map.cbegin(); }
		inline ConstIterator cend() const { return m_Map.cend(); }

	private:
		std::map<TKey, TValue> m_Map;

	};
}

template<typename TKey, typename TValue> struct std::tuple_size<Sphynx::HashMap<TKey, TValue>> : std::integral_constant<size_t, 2> {};
template<typename TKey, typename TValue> struct std::tuple_element<0, Sphynx::HashMap<TKey, TValue>> { using type = TKey; };
template<typename TKey, typename TValue> struct std::tuple_element<1, Sphynx::HashMap<TKey, TValue>> { using type = TValue; };

template<typename TKey, typename TValue> struct std::tuple_size<Sphynx::Map<TKey, TValue>> : std::integral_constant<size_t, 2> {};
template<typename TKey, typename TValue> struct std::tuple_element<0, Sphynx::Map<TKey, TValue>> { using type = TKey; };
template<typename TKey, typename TValue> struct std::tuple_element<1, Sphynx::Map<TKey, TValue>> { using type = TValue; };


#include "Reflection/Reflection.h"


SPX_REFLECT_TEMPLATE_CLASS_BEGIN(Sphynx::HashMap, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(AssociativeCollection)
SPX_REFLECT_TEMPLATE_CLASS_END(Sphynx::HashMap, SPHYNX_API)

SPX_REFLECT_TEMPLATE_CLASS_BEGIN(Sphynx::Map, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(AssociativeCollection)
SPX_REFLECT_TEMPLATE_CLASS_END(Sphynx::Map, SPHYNX_API)
