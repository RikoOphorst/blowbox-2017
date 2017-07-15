#pragma once

namespace blowbox
{
    /**
    * Enumerates all the different mouse buttons that blowbox supports.
    *
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
    * @brief Converts a GLFW_MOUSE_BUTTON to a blowbox::MouseButton
    * @author Riko Ophorst
    * @param[in] glfw_mouse_button A GLFW_MOUSE_BUTTON that should be converted to a blowbox::MouseButton
    */
    MouseButton GlfwMouseButtonToBlowboxMouseButton(int glfw_mouse_button);

    /**
    * @brief Converts a MouseButton_* to a GLFW_MOUSE_BUTTON_*
    * @author Riko Ophorst
    * @param[in] mouse_button A blowbox::MouseButton that should be converted to a GLFW_MOUSE_BUTTON
    */
    int BlowboxMouseButtonToGlfwMouseButton(MouseButton mouse_button);
}