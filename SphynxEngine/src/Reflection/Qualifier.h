#pragma once

#include "Core/Core.h"


namespace Sphynx
{
    namespace Reflection
    {
        struct SPHYNX_API Qualifier
        {
            enum Value : uint8_t
            {
                NONE		= 0,
                CONSTANT	= 1 << 0,
                VOLATILE	= 1 << 1,
                //MUTABLE 	= 1 << 2,
            };
            using Mask = typename std::underlying_type<Value>::type;
        };
    }
}