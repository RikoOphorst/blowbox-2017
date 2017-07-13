#pragma once

#include "util/eastl.h"
#include <EASTL/safe_ptr.h>

namespace blowbox
{
	template<typename T>
	using SafePtr = eastl::safe_ptr<T>;
}