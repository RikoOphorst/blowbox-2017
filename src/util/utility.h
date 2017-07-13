#pragma once

#include "util/eastl.h"
#include <EASTL/utility.h>

namespace blowbox
{
	template<typename T1, typename T2>
	using Pair = eastl::pair<T1, T2>;
}