#include "mouse_button.h"

#include <GLFW/glfw3.h>

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    MouseButton GlfwMouseButtonToBlowboxMouseButton(int glfw_mouse_button)
    {
        MouseButton blowbox_mouse_button = MouseButton_LEFT;

        switch (glfw_mouse_button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:        blowbox_mouse_button = MouseButton_LEFT;    break;
            case GLFW_MOUSE_BUTTON_MIDDLE:      blowbox_mouse_button = MouseButton_MIDDLE;  break;
            case GLFW_MOUSE_BUTTON_RIGHT:       blowbox_mouse_button = MouseButton_RIGHT;   break;
            case GLFW_MOUSE_BUTTON_4:           blowbox_mouse_button = MouseButton_4;       break;
            case GLFW_MOUSE_BUTTON_5:           blowbox_mouse_button = MouseButton_5;       break;
            case GLFW_MOUSE_BUTTON_6:           blowbox_mouse_button = MouseButton_6;       break;
            case GLFW_MOUSE_BUTTON_7:           blowbox_mouse_button = MouseButton_7;       break;
            case GLFW_MOUSE_BUTTON_8:           blowbox_mouse_button = MouseButton_8;       break;
        }

        return blowbox_mouse_button;
    }
    
    //------------------------------------------------------------------------------------------------------
    int BlowboxMouseButtonToGlfwMouseButton(MouseButton mouse_button)
    {
        int glfw_mouse_button = GLFW_MOUSE_BUTTON_LEFT;

        switch (mouse_button)
        {
            case MouseButton_LEFT:        glfw_mouse_button = GLFW_MOUSE_BUTTON_LEFT;    break;
            case MouseButton_MIDDLE:      glfw_mouse_button = GLFW_MOUSE_BUTTON_MIDDLE;  break;
            case MouseButton_RIGHT:       glfw_mouse_button = GLFW_MOUSE_BUTTON_RIGHT;   break;
            case MouseButton_4:           glfw_mouse_button = GLFW_MOUSE_BUTTON_4;       break;
            case MouseButton_5:           glfw_mouse_button = GLFW_MOUSE_BUTTON_5;       break;
            case MouseButton_6:           glfw_mouse_button = GLFW_MOUSE_BUTTON_6;       break;
            case MouseButton_7:           glfw_mouse_button = GLFW_MOUSE_BUTTON_7;       break;
            case MouseButton_8:           glfw_mouse_button = GLFW_MOUSE_BUTTON_8;       break;
        }

        return glfw_mouse_button;
    }
}