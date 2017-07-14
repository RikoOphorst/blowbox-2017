#pragma once

#include "util/eastl.h"
#include <EASTL/functional.h>

namespace blowbox
{
    /**
    * Wraps the EASTL function container. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL function.
    */
	template<typename Result, typename... Arguments>
	using FunctionWithArguments = eastl::function<Result(Arguments...)>;

    /**
    * Wraps the EASTL function container. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL function.
    */
	template<typename Result, typename Argument0>
	using FunctionWithArgument = eastl::function<Result(Argument0)>;

    /**
    * Wraps the EASTL function container. Refer to EASTL documentation for more information.
    *
    * @brief Typedef for wrapping the EASTL function.
    */
	template<typename Result>
	using Function = eastl::function<Result()>;
}