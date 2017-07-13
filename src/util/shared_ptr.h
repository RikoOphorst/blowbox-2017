#pragma once

#include "util/eastl.h"
#include <EASTL/shared_ptr.h>

namespace blowbox
{
	template<typename T>
	using SharedPtr = eastl::shared_ptr<T>;
}