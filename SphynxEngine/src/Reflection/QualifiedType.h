#pragma once

#include "Core/Core.h"
#include "Qualifier.h"
#include "ValueType.h"


namespace Sphynx
{
    namespace Reflection
    {
        struct Type;

        struct SPHYNX_API QualifiedType
        {
            const Type& Type;
            Qualifier::Mask Qualifiers;
            ValueType ValueType;
			size_t PointerIndirectionCount;
        };
    }
}