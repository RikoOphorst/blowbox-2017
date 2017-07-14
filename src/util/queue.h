#pragma once

#include "util/eastl.h"
#include <EASTL/queue.h>

namespace blowbox
{
    /**
    * Wraps the EASTL queue. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL queue.
    */
	template<typename T>
	using Queue = eastl::queue<T>;
}