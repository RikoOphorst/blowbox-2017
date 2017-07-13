#pragma once

#include "util/eastl.h"
#include <EASTL/bonus/ring_buffer.h>

namespace blowbox
{
	template<typename T>
	using RingBuffer = eastl::ring_buffer<T>;
}