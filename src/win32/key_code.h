#pragma once

namespace blowbox
{
    /**
    * This enumeration is to enumerate all different key codes that Blowbox supports.
    *
    * @brief All keycodes
    */
    enum KeyCode
    {
        KeyCode_SPACE,
        KeyCode_APOSTROPHE,
        KeyCode_COMMA,
        KeyCode_MINUS,
        KeyCode_PERIOD,
        KeyCode_SLASH,
        KeyCode_0,
        KeyCode_1,
        KeyCode_2,
        KeyCode_3,
        KeyCode_4,
        KeyCode_5,
        KeyCode_6,
        KeyCode_7,
        KeyCode_8,
        KeyCode_9,
        KeyCode_SEMICOLON,
        KeyCode_EQUAL,
        KeyCode_A,
        KeyCode_B,
        KeyCode_C,
        KeyCode_D,
        KeyCode_E,
        KeyCode_F,
        KeyCode_G,
        KeyCode_H,
        KeyCode_I,
        KeyCode_J,
        KeyCode_K,
        KeyCode_L,
        KeyCode_M,
        KeyCode_N,
        KeyCode_O,
        KeyCode_P,
        KeyCode_Q,
        KeyCode_R,
        KeyCode_S,
        KeyCode_T,
        KeyCode_U,
        KeyCode_V,
        KeyCode_W,
        KeyCode_X,
        KeyCode_Y,
        KeyCode_Z,
        KeyCode_LEFT_BRACKET,
        KeyCode_BACKSLASH,
        KeyCode_RIGHT_BRACKET,
        KeyCode_GRAVE_ACCENT,
        KeyCode_WORLD_1,
        KeyCode_WORLD_2,
        KeyCode_ESCAPE,
        KeyCode_ENTER,
        KeyCode_TAB,
        KeyCode_BACKSPACE,
        KeyCode_INSERT,
        KeyCode_DELETE,
        KeyCode_RIGHT,
        KeyCode_LEFT,
        KeyCode_DOWN,
        KeyCode_UP,
        KeyCode_PAGE_UP,
        KeyCode_PAGE_DOWN,
        KeyCode_HOME,
        KeyCode_END,
        KeyCode_CAPS_LOCK,
        KeyCode_SCROLL_LOCK,
        KeyCode_NUM_LOCK,
        KeyCode_PRINT_SCREEN,
        KeyCode_PAUSE,
        KeyCode_F1,
        KeyCode_F2,
        KeyCode_F3,
        KeyCode_F4,
        KeyCode_F5,
        KeyCode_F6,
        KeyCode_F7,
        KeyCode_F8,
        KeyCode_F9,
        KeyCode_F10,
        KeyCode_F11,
        KeyCode_F12,
        KeyCode_F13,
        KeyCode_F14,
        KeyCode_F15,
        KeyCode_F16,
        KeyCode_F17,
        KeyCode_F18,
        KeyCode_F19,
        KeyCode_F20,
        KeyCode_F21,
        KeyCode_F22,
        KeyCode_F23,
        KeyCode_F24,
        KeyCode_F25,
        KeyCode_KP_0,
        KeyCode_KP_1,
        KeyCode_KP_2,
        KeyCode_KP_3,
        KeyCode_KP_4,
        KeyCode_KP_5,
        KeyCode_KP_6,
        KeyCode_KP_7,
        KeyCode_KP_8,
        KeyCode_KP_9,
        KeyCode_KP_DECIMAL,
        KeyCode_KP_DIVIDE,
        KeyCode_KP_MULTIPLY,
        KeyCode_KP_SUBTRACT,
        KeyCode_KP_ADD,
        KeyCode_KP_ENTER,
        KeyCode_KP_EQUAL,
        KeyCode_LEFT_SHIFT,
        KeyCode_LEFT_CONTROL,
        KeyCode_LEFT_ALT,
        KeyCode_LEFT_SUPER,
        KeyCode_RIGHT_SHIFT,
        KeyCode_RIGHT_CONTROL,
        KeyCode_RIGHT_ALT,
        KeyCode_RIGHT_SUPER,
        KeyCode_MENU,
        KeyCode_UNKNOWN
    };

    /**
    * @brief Converts a GLFW_KEY_* to a KeyCode_*
    * @param[in] glfw_key A GLFW_KEY that you want to convert to a blowbox::KeyCode
    */
    KeyCode GlfwKeyToBlowboxKeyCode(int glfw_key);

    /**
    * @brief Converts a KeyCode_* to a GLFW_KEY_*
    * @param[in] key A blowbox::KeyCode that should be converted to a GLFW_KEY
    */
    int BlowboxKeyCodeToGlfwKey(KeyCode key);
}