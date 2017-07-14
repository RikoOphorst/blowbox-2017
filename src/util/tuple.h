#pragma once

#include "util/eastl.h"
#include <EASTL/tuple.h>

namespace blowbox
{
    /**
    * Wraps the EASTL tuple. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL tuple.
    */
	template<typename... Ts>
	using Tuple = eastl::tuple<Ts...>;
}