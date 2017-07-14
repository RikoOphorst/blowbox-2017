#pragma once

#include "util/resolution.h"

#include "util/eastl.h"
#include "util/string.h"

namespace blowbox
{
    /**
    * This struct is meant to configure the BlowboxCore when it gets instantiated. 
    * If you want to change a blowbox setting, this is the place to do it. 
    *
    * @brief Configuration of Blowbox.
    */
    struct BlowboxConfig
    {
        /**
        * @brief Constructs a BlowboxConfig.
        */
        BlowboxConfig();

        /**
        * @brief Constructs a BlowboxConfig from command line arguments.
        * @param[in] argc The number of arguments in argv. This is passed in by the main function (int main(int argc, char* argv)).
        * @param[in] argv The actual value of the command line arguments. This is passed in by the main function (int main(int argc, char* argv)).
        */
        BlowboxConfig(int argc, char* argv);

        String window_title;            //!< The title of the main Window that the BlowboxCore instantiates.
        Resolution window_resolution;   //!< The resolution of the main Window that the BlowboxCore instantiates.
        String window_icon_file_path;   //!< A file path to an image that should be used as the icon for the main Window. Image should be 16x16, 32x32 or 48x48.
        bool enable_imgui;              //!< Whether ImGui should be enabled.
        bool toggle_deferred;           //!< Toggles whether Blowbox renders using a deferred renderer or a forward renderer.
    };
}