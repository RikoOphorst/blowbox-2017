#pragma once

#include "util/string.h"

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
    /**
    * @struct blowbox::Resolution
    * @author Riko Ophorst
    * @brief Stores resolution information in pixels
    */
    struct Resolution
    {
        /**
        * @brief Constructs an empty resolution (0, 0)
        * @author Riko Ophorst
        */
        Resolution();

        /**
        * @brief Constructs a resolution given a width & height
        * @author Riko Ophorst
        * @param[in] width  The width of the resolution.
        * @param[in] height The height of the resolution.
        */
        Resolution(int width, int height);

        /**
        * @brief Calculates the aspect ratio of this resolution
        * @author Riko Ophorst
        */
        float GetAspectRatio();

        /**
        * @brief Converts this resolution to a printable string
        */
        String ToString();

        int width; //!< The width of the resolution
        int height; //!< The height of the resolution
    };
}