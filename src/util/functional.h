#pragma once

#include "util/eastl.h"
#include <EASTL/functional.h>

namespace blowbox
{
	template<typename Result, typename... Arguments>
	using FunctionWithArguments = eastl::function<Result(Arguments...)>;

	template<typename Result, typename Argument0>
	using FunctionWithArgument = eastl::function<Result(Argument0)>;

	template<typename Result>
	using Function = eastl::function<Result()>;
}