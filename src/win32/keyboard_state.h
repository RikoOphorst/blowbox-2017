#pragma once

#include "win32/key_code.h"

#include "util/map.h"

namespace blowbox
{
    /**
    * This class keeps track of the state of the keyboard. Normally, you would
    * find access to instances of this class through Window instances, because
    * every Window has different keyboard states. Access the KeyboardState by
    * calling Window::GetKeyboardState().
    *
    * @brief Keeps track of the state of the keyboard.
    */
    class KeyboardState
    {
        friend class GLFWManager;
        friend class Window;
        friend class ImGuiManager;
    public:
        /**
        * Every key on your keyboard has a certain state. This struct
        * keeps track of that.
        *
        * @brief Keeps track of the state of a key on the keyboard.
        */
        struct KeyState
        {
            bool down = false;              //!< Whether the key is currently held down.
            bool pressed = false;           //!< Whether the key is currently pressed. (only true for 1 frame!)
            bool released = false;          //!< Whether the key is currently released. (only true for 1 frame!)
            bool modifier_control = false;  //!< Whether the control-modifier was pressed together with this key.
            bool modifier_alt = false;      //!< Whether the alt-modifier was pressed together with this key.
            bool modifier_shift = false;    //!< Whether the shift-modifier was pressed together with this key.
        };

        KeyboardState();
        ~KeyboardState();

        /**
        * @brief Find out whether a key is down or not.
        * @param[in] key The key you want to know the down-state of.
        * @returns Whether the key is down or not.
        */
        bool GetKey(KeyCode key);

        /**
        * @brief Find out whether a key is down or not.
        * @param[in] key The key you want to know the down-state of.
        * @returns Whether the key is down or not.
        */
        bool GetKeyDown(KeyCode key);

        /**
        * @brief Find out whether a key is pressed or not.
        * @param[in] key The key you want to know the pressed-state of.
        * @returns Whether the key is pressed or not.
        */
        bool GetKeyPressed(KeyCode key);

        /**
        * @brief Find out whether a key is released or not.
        * @param[in] key The key you want to know the released-state of.
        * @returns Whether the key is released or not.
        */
        bool GetKeyReleased(KeyCode key);

        /**
        * @brief Retrieves the full state of a key.
        * @param[in] key The key you want to know the full KeyState of.
        * @returns The full KeyState of a key.
        */
        const KeyState& GetKeyState(KeyCode key);

    protected:
        /**
        * @brief Changes the press-state of a key to true.
        * @param[in] key The key to be changed.
        */
        void SetKeyPressed(KeyCode key);

        /**
        * @brief Changes the release-state of a key to true.
        * @param[in] key The key to be changed.
        */
        void SetKeyReleased(KeyCode key);

        /**
        * @brief Changes the key modifiers of a key to true.
        * @param[in] key The key to be changed.
        * @param[in] modifiers The modifiers that were pressed when the key was pressed.
        */
        void SetKeyModifiers(KeyCode key, int modifiers);

        /** @brief Resets all keys by changing their press & release states to false. */
        void ResetKeys();
    private:
        Map<KeyCode, KeyState> key_states_; //!< A map of KeyCodes to KeyStates.
    };
}