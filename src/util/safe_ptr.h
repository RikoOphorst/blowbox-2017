#pragma once

#include "util/eastl.h"
#include <EASTL/safe_ptr.h>

namespace blowbox
{
    /**
    * Wraps the EASTL safe_ptr. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL safe_ptr.
    */
	template<typename T>
	using SafePtr = eastl::safe_ptr<T>;
}