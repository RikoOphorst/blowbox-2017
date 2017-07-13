#pragma once

#include "util/eastl.h"
#include <EASTL/deque.h>

namespace blowbox
{
	template<typename T>
	using Deque = eastl::deque<T>;
}