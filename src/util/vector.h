#pragma once

#include "util/eastl.h"
#include <EASTL/vector.h>

namespace blowbox
{
	template<typename T>
	using Vector = eastl::vector<T>;
}