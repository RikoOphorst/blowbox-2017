#pragma once

#include "util/eastl.h"
#include <EASTL/string.h>

namespace blowbox
{
    /**
    * Wraps the EASTL string. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL string.
    */
	using String = eastl::string;

    /**
    * Wraps the EASTL wstring. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL wstring.
    */
	using WString = eastl::wstring;
}