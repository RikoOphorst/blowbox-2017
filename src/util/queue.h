#pragma once

#include "util/eastl.h"
#include <EASTL/queue.h>

namespace blowbox
{
	template<typename T>
	using Queue = eastl::queue<T>;
}