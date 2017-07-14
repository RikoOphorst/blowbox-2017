#include "blowbox_config.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    BlowboxConfig::BlowboxConfig() :
        window_title("Blowbox"),
        window_resolution(1280, 720),
        window_icon_file_path("icon.png"),
        enable_imgui(true),
        toggle_deferred(false)
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    BlowboxConfig::BlowboxConfig(int argc, char* argv) :
        window_title("Blowbox"),
        window_resolution(1280, 720),
        window_icon_file_path("icon.png"),
        enable_imgui(true),
        toggle_deferred(false)
    {

    }
}