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
		}
	}
}
