#pragma once

#include "util/eastl.h"
#include <EASTL/shared_ptr.h>

namespace blowbox
{
    /**
    * Wraps the EASTL shared_ptr. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL shared_ptr.
    */
	template<typename T>
	using SharedPtr = eastl::shared_ptr<T>;
}