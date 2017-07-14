#pragma once

#include "util/eastl.h"
#include <EASTL/unique_ptr.h>

namespace blowbox
{
    /**
    * Wraps the EASTL unique_ptr. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL unique_ptr.
    */
	template<typename T>
	using UniquePtr = eastl::unique_ptr<T>;
}