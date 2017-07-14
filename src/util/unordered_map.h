#pragma once

#include "util/eastl.h"
#include <EASTL/unordered_map.h>

namespace blowbox
{
    /**
    * Wraps the EASTL unordered_map. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL unordered_map.
    */
	template<typename Key, typename T>
	using UnorderedMap = eastl::unordered_map<Key, T>;
}