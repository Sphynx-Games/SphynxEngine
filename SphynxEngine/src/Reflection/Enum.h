#pragma once

#include "Core/Core.h"
#include "Type.h"
#include <string>


namespace Sphynx
{
	namespace Reflection
	{
		struct SPHYNX_API Enum : public Type
		{
			struct SPHYNX_API Entry
			{
				std::string Name;
				int64_t Value;
			};

			const Entry* Values;
			size_t Count;

			inline const Entry* begin() { return Values; }
			inline const Entry* begin() const { return Values; }
			inline const Entry* end() { return Values + Count; }
			inline const Entry* end() const { return Values + Count; }

			template<typename T>
			inline int64_t GetValue(T e) const
			{
				SPX_CORE_ASSERT(&GetEnum<T>() == this);

				int64_t value = (int64_t)e;
				auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Value == value; });
				SPX_CORE_ASSERT(it != Values + Count);

				return it->Value;
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

		};
	}
}
