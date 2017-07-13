#pragma once

#include "util/eastl.h"
#include <EASTL/weak_ptr.h>

namespace blowbox
{
	template<typename T>
	using WeakPtr = eastl::weak_ptr<T>;
}