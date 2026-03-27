#include "spxpch.h"
#include "Common.h"


namespace Sphynx
{
	namespace Reflection
	{
		namespace CommonAttribute
		{
			InternalComponent::~InternalComponent()
			{
				deleter();
				deleter = nullptr;
			}

			size_t InternalComponent::GetTypeID() const
			{
				return TypeID<InternalComponent>::ID;
			}
		}
	}
}

