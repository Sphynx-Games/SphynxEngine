#include "Reflection/Enum.h"


namespace Sphynx
{
	namespace Reflection
	{
		Enum::Entry::Entry(const std::string& name, int64_t value) :
			Name(name),
			Value(value),
			Attributes()
		{

		}

		Enum::Entry::Entry(const Entry& other) :
			Name(other.Name),
			Value(other.Value),
			Attributes(other.Attributes)
		{

		}

		Enum::Entry::Entry(Entry&& other) :
			Name(std::move(other.Name)),
			Value(std::move(other.Value)),
			Attributes(std::move(other.Attributes))
		{

		}

		Enum::Entry::~Entry()
		{

		}

		const Enum::Entry* Enum::begin()
		{
			return Values;
		}

		const Enum::Entry* Enum::begin() const
		{
			return Values;
		}

		const Enum::Entry* Enum::end()
		{
			return Values + Count;
		}

		const Enum::Entry* Enum::end() const
		{
			return Values + Count;
		}

		int64_t Enum::GetValue(const void* addr) const
		{
			SPX_CORE_ASSERT(m_GetValueFunc != nullptr, "Function is nullptr!!");
			return m_GetValueFunc(addr);
		}

		void Enum::SetValue(void* addr, int64_t value) const
		{
			SPX_CORE_ASSERT(m_SetValueFunc != nullptr, "Function is nullptr!!");
			m_SetValueFunc(addr, value);
		}

		const std::string& Enum::GetName(const void* addr) const
		{
			int64_t value = GetValue(addr);
			auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Value == value; });
			SPX_CORE_ASSERT(it != Values + Count);

			return it->Name;
		}

		void Enum::SetName(void* addr, const std::string& name) const
		{
			auto it = std::find_if(Values, Values + Count, [&](const Entry& entry) { return entry.Name == name; });
			SPX_CORE_ASSERT(it != Values + Count);

			SetValue(addr, it->Value);
		}
	}
}