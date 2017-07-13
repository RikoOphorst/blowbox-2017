#pragma once

#include "util/eastl.h"
#include <EASTL/unique_ptr.h>

namespace blowbox
{
	template<typename T>
	using UniquePtr = eastl::unique_ptr<T>;
}