#pragma once

#include "util/eastl.h"
#include <EASTL/intrusive_ptr.h>

namespace blowbox
{
    /**
    * Wraps the EASTL intrusive_ptr. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL intrusive_ptr.
    */
	template<typename T>
	using IntrusivePtr = eastl::intrusive_ptr<T>;
}