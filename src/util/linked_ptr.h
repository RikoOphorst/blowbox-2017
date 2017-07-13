#pragma once

#include "util/eastl.h"
#include <EASTL/linked_ptr.h>

namespace blowbox
{
	template<typename T>
	using LinkedPtr = eastl::linked_ptr<T>;
}