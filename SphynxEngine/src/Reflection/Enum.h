#pragma once

#include "Core/Core.h"
#include "Type.h"
#include "Attribute.h"
#include <string>


namespace Sphynx
{
	namespace Reflection
	{
		struct SPHYNX_API Enum : public Type
		{
		public:
			struct SPHYNX_API Entry
			{
				std::string Name;
				int64_t Value;

				std::vector<Attribute*> Attributes;
			};

			template<typename T>
			Enum(const Type& type, const Entry* values, size_t count, const Attribute* const* attributes, size_t attributesCount, details::Tag<T>) :
				Type(type),
				Values(values),
				Count(count),
				Attributes(attributes),
				AttributesCount(attributesCount),
				m_GetValueFunc([](const void* addr) -> int64_t { return *((T*)addr); }),
				m_SetValueFunc([](void* addr, int64_t value) -> void { *((T*)addr) = (T)value; })
			{}

			inline const Entry* begin() { return Values; }
			inline const Entry* begin() const { return Values; }
			inline const Entry* end() { return Values + Count; }
			inline const Entry* end() const { return Values + Count; }

			inline int64_t GetValue(const void* addr) const
			{
				SPX_CORE_ASSERT(m_GetValueFunc != nullptr, "Function is nullptr!!");
				return m_GetValueFunc(addr);
			}

			template<typename T>
			inline int64_t GetValue(T e) const
			{
				SPX_CORE_ASSERT(&GetEnum<T>() == this);

				int64_t value = (int64_t)e;
				auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Value == value; });
				SPX_CORE_ASSERT(it != Values + Count);

				return it->Value;
			}

			inline void SetValue(void* addr, int64_t value) const
			{
				SPX_CORE_ASSERT(m_SetValueFunc != nullptr, "Function is nullptr!!");
				m_SetValueFunc(addr, value);
			}

			inline const std::string& GetName(const void* addr) const
			{
				int64_t value = GetValue(addr);
				auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Value == value; });
				SPX_CORE_ASSERT(it != Values + Count);

				return it->Name;
			}

			template<typename T>
			inline const std::string& GetName(T e) const
			{
				SPX_CORE_ASSERT(&GetEnum<T>() == this);

				int64_t value = (int64_t)e;
				auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Value == value; });
				SPX_CORE_ASSERT(it != Values + Count);

				return it->Name;
			}

			inline void SetName(void* addr, const std::string& name) const
			{
				auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Name == name; });
				SPX_CORE_ASSERT(it != Values + Count);

				SetValue(addr, it->Value);
			}

		public:
			const Entry* Values;
			size_t Count;

			const Attribute* const* Attributes;
			size_t AttributesCount;

		private:
			int64_t (*m_GetValueFunc)(const void*);
			void (*m_SetValueFunc)(void*, int64_t);
		};
	}
}
