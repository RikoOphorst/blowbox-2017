#pragma once

#include "util/eastl.h"
#include <EASTL/tuple.h>

namespace blowbox
{
	template<typename... Ts>
	using Tuple = eastl::tuple<Ts...>;
}