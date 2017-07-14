#pragma once

#include "util/eastl.h"
#include <EASTL/bonus/ring_buffer.h>

namespace blowbox
{
    /**
    * Wraps the EASTL ring_buffer. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL ring_buffer.
    */
	template<typename T>
	using RingBuffer = eastl::ring_buffer<T>;
}