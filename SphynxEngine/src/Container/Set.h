#pragma once

#include "Core/Core.h"
#include "Array.h"
#include <unordered_set>


namespace Sphynx
{
	template <typename T>
	class SPHYNX_API Set
	{
	public:
		using Iterator = typename std::unordered_set<T>::iterator;
		using ConstIterator = typename std::unordered_set<T>::const_iterator;

	public:
		Set() : m_Set() {}

		Set(size_t initialCapacity) : Set()
		{
			m_Set.reserve(initialCapacity);
		}

		Set(const Set& other) : Set(other.Size())
		{
			m_Set = other.m_Set;
			Append(other);
		}

		Set(const Array<T>& other) : Set()
		{
			Append(other);
		}

		Set(Set&& other)
		{
			m_Set = std::move(other.m_Set);
			other.RemoveAll();
		}

		Set(Array<T>&& other) // TODO: REVISE THIS
		{
			m_Set = std::move(ToSet(other).m_Set);
			other.RemoveAll();
		}

		~Set() = default;

		inline void Add(const T& value) { m_Set.insert(value); }
		inline void Add(T&& value) { m_Set.insert(value); }

		void Append(const Set& set)
		{
			for (const T& value : set)
			{
				Add(value);
			}
		}

		void Append(const Array<T>& array)
		{
			for (const T& value : array)
			{
				Add(value);
			}
		}

		template <typename ...Args>
		inline T& Emplace(Args&&... args)
		{
			return m_Set.emplace(std::forward<Args>(args)...);
		}

		inline void Remove(const T& value) { m_Set.erase(value); }
		inline void RemoveAll() { m_Set.clear(); }

		inline const T& operator [] (uint64_t index) const { return m_Set.at(index); }
		inline T& operator [] (uint64_t index) { return m_Set.at(index); }

		Set& operator = (const Set& other)
		{
			RemoveAll();
			Append(other);
			return *this;
		}

		Set& operator = (Set&& other)
		{
			RemoveAll();
			m_Set = std::move(other.m_Set);
			other.RemoveAll();
			return *this;
		}

		inline bool Contains(const T & value) const { return m_Set.count(value) != 0; }
		inline size_t Size() const { return m_Set.size(); }
		inline bool IsEmpty() const { return m_Set.empty(); }

		inline Iterator begin() { return m_Set.begin(); }
		inline Iterator end() { return m_Set.end(); }
		inline ConstIterator begin() const { return m_Set.begin(); }
		inline ConstIterator end() const { return m_Set.end(); }

		inline ConstIterator cbegin() const { return m_Set.cbegin(); }
		inline ConstIterator cend() const { return m_Set.cend(); }

		static Set ToSet(const Array<T>& other)
		{
			Set set;
			set.Append(other);
			return set;
		}

	private:
		std::unordered_set<T> m_Set;

	};
}