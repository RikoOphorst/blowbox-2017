#pragma once

#include "util/eastl.h"
#include <EASTL/priority_queue.h>

namespace blowbox
{
	template<typename T>
	using PriorityQueue = eastl::priority_queue<T>;
}