#pragma once

#include "util/eastl.h"
#include <EASTL/unordered_map.h>

namespace blowbox
{
	template<typename Key, typename T>
	using UnorderedMap = eastl::unordered_map<Key, T>;
}