#include "Reflection/Attribute.h"


namespace Sphynx
{
	namespace Reflection
	{
		Attribute::Attribute() = default;
		Attribute::~Attribute() = default;

		namespace CommonAttribute
		{
			POD::~POD() = default;

			void POD::CopyTo(const void* source, void* dest) const
			{
				m_CopyToFunction(source, dest);
			}

			size_t POD::GetTypeID() const
			{
				return TypeID<POD>::ID;
			}

			size_t Description::GetTypeID() const
			{
				return TypeID<Description>::ID;
			}

			size_t AssociativeCollection::GetTypeID() const
			{
				return TypeID<AssociativeCollection>::ID;
			}

			size_t IndexedCollection::GetTypeID() const
			{
				return TypeID<IndexedCollection>::ID;
			}

			size_t Range::GetTypeID() const
			{
				return TypeID<Range>::ID;
			}

		}
	}
}

