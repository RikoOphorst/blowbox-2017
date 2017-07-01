#pragma once

#include "core/eastl.h"

#include <EASTL/functional.h>

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
	class BlowboxCore
	{
	public:
		BlowboxCore();
		~BlowboxCore();

		void Init();
	};
}