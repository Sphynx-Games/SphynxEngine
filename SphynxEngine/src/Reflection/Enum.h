#pragma once

#include "Core/Core.h"
#include "Type.h"
#include "Attribute.h"

#include <vector>
#include <string>
#include <algorithm>


namespace Sphynx
{
	namespace Reflection
	{
		struct SPHYNX_API Enum : public Type
		{
		public:
			struct SPHYNX_API Entry
			{
				Entry(const std::string& name, int64_t value);
				Entry(const Entry& other);
				Entry(Entry&& other);
				~Entry();

				std::string Name;
				int64_t Value;

				std::vector<Attribute*> Attributes;
			};

			template<typename T>
			Enum(const Type& type, const Entry* values, size_t count, const Attribute* const* attributes, size_t attributesCount, details::Tag<T>) :
				Type(type),
				UnderlyingType(GetType<T>()),
				Values(values),
				Count(count),
				Attributes(attributes),
				AttributesCount(attributesCount),
				m_GetValueFunc([](const void* addr) -> int64_t { return *((T*)addr); }),
				m_SetValueFunc([](void* addr, int64_t value) -> void { *((T*)addr) = (T)value; })
			{}

			const Entry* begin();
			const Entry* begin() const;
			const Entry* end();
			const Entry* end() const;

			int64_t GetValue(const void* addr) const;

			template<typename T>
			int64_t GetValue(T e) const
			{
				SPX_CORE_ASSERT(&GetEnum<T>() == this);

				int64_t value = (int64_t)e;
				auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Value == value; });
				SPX_CORE_ASSERT(it != Values + Count);

				return it->Value;
			}

			void SetValue(void* addr, int64_t value) const;
			const std::string& GetName(const void* addr) const;

			template<typename T>
			const std::string& GetName(T e) const
			{
				SPX_CORE_ASSERT(&GetEnum<T>() == this);

				int64_t value = (int64_t)e;
				auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Value == value; });
				SPX_CORE_ASSERT(it != Values + Count);

				return it->Name;
			}

			void SetName(void* addr, const std::string& name) const;

		public:
			const Type& UnderlyingType;

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
