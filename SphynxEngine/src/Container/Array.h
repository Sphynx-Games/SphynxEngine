#pragma once

#include "Core/Core.h"
#include <vector>


namespace Sphynx
{
	template <typename T>
	class SPHYNX_API Array
	{
	public:
		using Iterator = typename std::vector<T>::iterator;
		using ConstIterator = typename std::vector<T>::const_iterator;
		using ReverseIterator = typename std::vector<T>::reverse_iterator;
		using ConstReverseIterator = typename std::vector<T>::const_reverse_iterator;

	public:
		Array() : m_Array() {}

		Array(size_t initialCapacity) : Array()
		{
			m_Array.reserve(initialCapacity);
		}

		Array(const Array& other) : Array(other.Size())
		{
			Append(other);
		}

		Array(Array&& other)
		{
			m_Array = std::move(other.m_Array);
			other.RemoveAll();
		}

		~Array() = default;

		inline void Add(const T& value) { m_Array.push_back(value); }
		inline void Add(T&& value) { m_Array.push_back(value); }

		void Append(const Array& array)
		{
			for (const T& value : array)
			{
				Add(value);
			}
		}

		template <typename ...Args>
		inline T& Emplace(Args&&... args)
		{
			return m_Array.emplace_back(std::forward<Args>(args)...);
		}

		template <typename ...Args>
		inline Iterator EmplaceAt(uint64_t index, Args&&... args)
		{
			SPX_CORE_ASSERT(index >= 0 && index < Size(), "index out of range!!");

			return m_Array.emplace(m_Array.begin() + index, std::forward<Args>(args)...);
		}

		template <typename ...Args>
		inline Iterator EmplaceAt(ConstIterator it, Args&&... args)
		{
			return m_Array.emplace(it, std::forward<Args>(args)...);
		}

		void Remove(const T& value)
		{
			auto it = std::find(m_Array.begin(), m_Array.end(), value);

			// If element is found found, erase it 
			if (it != m_Array.end())
			{
				m_Array.erase(it);
			}
		}

		void RemoveAt(uint64_t index)
		{
			if (index >= Size()) return;

			auto it = m_Array.begin() + index;
			m_Array.erase(it);
		}

		inline void RemoveAll() { m_Array.clear(); }

		inline const T& Get(uint64_t index) const { return m_Array.at(index); }
		inline T& Get(uint64_t index) { return m_Array.at(index); }

		inline const T& operator [] (uint64_t index) const { return m_Array.at(index); }
		inline T& operator [] (uint64_t index) { return m_Array.at(index); }

		Array& operator = (const Array& other) 
		{
			RemoveAll();
			Append(other);
			return *this;
		}

		Array& operator = (Array&& other)
		{
			RemoveAll();
			m_Array = std::move(other.m_Array);
			other.RemoveAll();
			return *this;
		}

		size_t IndexOf(const T& value)
		{
			auto it = std::find(m_Array.begin(), m_Array.end(), value);
			return std::distance(m_Array.begin(), it);
		}

		inline bool Contains(const T& value) const { return std::find(m_Array.begin(), m_Array.end(), value) != m_Array.end(); }
		inline size_t Size() const { return m_Array.size(); }
		inline bool IsEmpty() const { return m_Array.empty(); }

		inline const T* Data() const { return m_Array.data(); }
		inline T* Data() { return m_Array.data(); }

		inline Iterator begin() { return m_Array.begin(); }
		inline Iterator end() { return m_Array.end(); }

		inline ConstIterator begin() const { return m_Array.begin(); }
		inline ConstIterator end() const { return m_Array.end(); }
		inline ConstIterator cbegin() const { return m_Array.cbegin(); }
		inline ConstIterator cend() const { return m_Array.cend(); }

		inline ReverseIterator rbegin() { return m_Array.rbegin(); }
		inline ReverseIterator rend() { return m_Array.rend(); }

		inline ConstReverseIterator rbegin() const { return m_Array.rbegin(); }
		inline ConstReverseIterator rend() const { return m_Array.rend(); }

	private:
		std::vector<T> m_Array;
	};
}