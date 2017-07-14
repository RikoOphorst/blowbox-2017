#pragma once

#include "util/eastl.h"
#include <EASTL/map.h>

namespace blowbox
{
    /**
    * Wraps the EASTL map. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL map.
    */
	template<typename Key, typename T>
	using Map = eastl::map<Key, T>;
}