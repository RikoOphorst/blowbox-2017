#pragma once

#include "util/eastl.h"
#include <EASTL/intrusive_ptr.h>

namespace blowbox
{
	template<typename T>
	using IntrusivePtr = eastl::intrusive_ptr<T>;
}