#pragma once

#include "util/eastl.h"
#include <EASTL/scoped_ptr.h>

namespace blowbox
{
    /**
    * Wraps the EASTL scoped_ptr. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL scoped_ptr.
    */
	template<typename T>
	using ScopedPtr = eastl::scoped_ptr<T>;
}