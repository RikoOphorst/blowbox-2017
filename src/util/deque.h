#pragma once

#include "util/eastl.h"
#include <EASTL/deque.h>

namespace blowbox
{
    /**
    * A Deque is a double ended queue. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL Deque.
    */
	template<typename T>
	using Deque = eastl::deque<T>;
}