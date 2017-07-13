#pragma once

#include "win32/key_code.h"

#include "util/map.h"

namespace blowbox
{
    class KeyboardState
    {
        friend class GLFWManager;
        friend class Window;
        friend class ImGuiManager;
    public:
        struct KeyState
        {
            bool down = false;
            bool pressed = false;
            bool released = false;
            bool modifier_control = false;
            bool modifier_alt = false;
            bool modifier_shift = false;
        };

        KeyboardState();
        ~KeyboardState();

        bool GetKey(KeyCode key);
        bool GetKeyDown(KeyCode key);
        bool GetKeyPressed(KeyCode key);
        bool GetKeyReleased(KeyCode key);
        const KeyState& GetKeyState(KeyCode key);

    protected:
        void SetKeyPressed(KeyCode key);
        void SetKeyReleased(KeyCode key);
        void SetKeyModifiers(KeyCode key, int modifiers);

        void ResetKeys();
    private:
        Map<KeyCode, KeyState> key_states_;
    };
}