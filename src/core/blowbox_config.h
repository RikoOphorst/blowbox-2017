#pragma once

#include "util/resolution.h"

#include "core/eastl_required.h"
#include <EASTL/string.h>

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
	/**
	* @struct blowbox::BlowboxConfig
	* @author Riko Ophorst
	* @brief Configuration of blowbox. This is used as a parameter in the constructor of BlowboxCore.
	*/
	struct BlowboxConfig
	{
		BlowboxConfig();

		eastl::string window_title;
		Resolution window_resolution;
	};
}