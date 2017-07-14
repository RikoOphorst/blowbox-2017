#pragma once

#include "util/eastl.h"
#include <EASTL/stack.h>

namespace blowbox
{
    /**
    * Wraps the EASTL stack. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL stack.
    */
	template<typename T>
	using Stack = eastl::stack<T>;
}