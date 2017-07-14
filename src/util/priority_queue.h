#pragma once

#include "util/eastl.h"
#include <EASTL/priority_queue.h>

namespace blowbox
{
    /**
    * Wraps the EASTL priority_queue. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL priority_queue.
    */
	template<typename T>
	using PriorityQueue = eastl::priority_queue<T>;
}