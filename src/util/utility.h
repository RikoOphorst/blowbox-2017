#pragma once

#include "util/eastl.h"
#include <EASTL/utility.h>

namespace blowbox
{
    /**
    * Wraps the EASTL pair. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL pair.
    */
	template<typename T1, typename T2>
	using Pair = eastl::pair<T1, T2>;
}