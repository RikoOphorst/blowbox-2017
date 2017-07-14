#pragma once

#include "util/eastl.h"
#include <EASTL/weak_ptr.h>

namespace blowbox
{
    /**
    * Wraps the EASTL weak_ptr. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL weak_ptr.
    */
	template<typename T>
	using WeakPtr = eastl::weak_ptr<T>;
}