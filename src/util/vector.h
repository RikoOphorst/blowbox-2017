#pragma once

#include "util/eastl.h"
#include <EASTL/vector.h>

namespace blowbox
{
    /**
    * Wraps the EASTL vector. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL vector.
    */
	template<typename T>
	using Vector = eastl::vector<T>;
}