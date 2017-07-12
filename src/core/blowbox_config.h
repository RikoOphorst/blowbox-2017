#pragma once

#include "util/resolution.h"

#include "core/eastl.h"
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
        /**
        * @brief Constructs the blowbox configuration
        * @author Riko Ophorst
        */
        BlowboxConfig();

        eastl::string window_title; //!< The title of the main Window that the BlowboxCore instantiates
        Resolution window_resolution; //!< The resolution of the main Window that the BlowboxCore instantiates
    };
}