#pragma once

#include "Core/Core.h"


namespace Sphynx
{
    namespace Reflection
    {
        enum class SPHYNX_API ValueType : uint8_t
        {
            VALUE,
            LVALUE_REFERENCE,
            RVALUE_REFERENCE,
        };
    }
}