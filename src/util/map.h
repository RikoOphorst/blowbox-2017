#pragma once

#include "util/eastl.h"
#include <EASTL/map.h>

namespace blowbox
{
	template<typename Key, typename T>
	using Map = eastl::map<Key, T>;
}