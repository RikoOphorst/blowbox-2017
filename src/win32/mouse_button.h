#pragma once

namespace blowbox
{
    /**
    * @enum blowbox::MouseButton
    * @author Riko Ophorst
    * @brief All mouse buttons
    */
    enum MouseButton
    {
        MouseButton_LEFT,
        MouseButton_MIDDLE,
        MouseButton_RIGHT,
        MouseButton_4,
        MouseButton_5,
        MouseButton_6,
        MouseButton_7,
        MouseButton_8
    };


    /**
    * @brief Converts a GLFW_MOUSE_BUTTON_* to a MouseButton_*
    * @author Riko Ophorst
    * @param[in] glfw_key A GLFW_MOUSE_BUTTON_*
    */
    MouseButton GlfwMouseButtonToBlowboxMouseButton(int glfw_mouse_button);

    /**
    * @brief Converts a MouseButton_* to a GLFW_MOUSE_BUTTON_*
    * @author Riko Ophorst
    * @param[in] key A MouseButton_*
    */
    int BlowboxMouseButtonToGlfwMouseButton(MouseButton mouse_button);
}