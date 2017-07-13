#pragma once

#include "util/eastl.h"
#include <EASTL/scoped_ptr.h>

namespace blowbox
{
	template<typename T>
	using ScopedPtr = eastl::scoped_ptr<T>;
}