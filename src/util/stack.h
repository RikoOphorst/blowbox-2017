#pragma once

#include "util/eastl.h"
#include <EASTL/stack.h>

namespace blowbox
{
	template<typename T>
	using Stack = eastl::stack<T>;
}