#pragma once

#include "util/eastl.h"
#include <EASTL/linked_ptr.h>

namespace blowbox
{
    /**
    * Wraps the EASTL linked_ptr. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL linked_ptr.
    */
	template<typename T>
	using LinkedPtr = eastl::linked_ptr<T>;
}