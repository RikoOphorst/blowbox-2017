#pragma once

#include "win32/mouse_button.h"
#include "util/map.h"

#include <DirectXMath.h>

namespace blowbox
{
    /**
    * This class keeps track of the state of the mouse. Normally, you would
    * find access to instances of this class through Window instances, because
    * every Window has different mouse states. Access the MouseState by
    * calling Window::GetMouseState().
    *
    * @brief Keeps track of the state of the mouse.
    */
    class MouseState
    {
        friend class Window;
        friend class GLFWManager;
        friend class ImGuiManager;
    public:
        /**
        * Every key on your mouse has a certain state. This struct
        * keeps track of that.
        *
        * @brief Keeps track of the state of a button on the mouse.
        */
        struct MouseButtonState
        {
            bool down = false;              //!< Whether the key is currently held down.
            bool pressed = false;           //!< Whether the key is currently pressed. (only true for 1 frame!)
            bool released = false;          //!< Whether the key is currently released. (only true for 1 frame!)
            bool modifier_control = false;  //!< Whether the control-modifier was pressed together with this key.
            bool modifier_alt = false;      //!< Whether the alt-modifier was pressed together with this key.
            bool modifier_shift = false;    //!< Whether the shift-modifier was pressed together with this key.
        };

        MouseState();
        ~MouseState();

        /**
        * @brief Find out whether a button is down or not.
        * @param[in] button The key you want to know the down-state of.
        * @returns Whether the button is down or not.
        */
        bool GetButton(MouseButton button);

        /**
        * @brief Find out whether a button is down or not.
        * @param[in] button The key you want to know the down-state of.
        * @returns Whether the button is down or not.
        */
        bool GetButtonDown(MouseButton button);

        /**
        * @brief Find out whether a button is pressed or not.
        * @param[in] button The key you want to know the pressed-state of.
        * @returns Whether the button is pressed or not.
        */
        bool GetButtonPressed(MouseButton button);

        /**
        * @brief Find out whether a button is released or not.
        * @param[in] button The key you want to know the released-state of.
        * @returns Whether the button is released or not.
        */
        bool GetButtonReleased(MouseButton button);

        /** @returns The position of the mouse relative to the window. */
        const DirectX::XMFLOAT2& GetMousePosition() const;
        /** @returns The delta position of the mouse relative to the window. (i.e. how much it has moved since the last frame) */
        const DirectX::XMFLOAT2& GetMousePositionDelta() const;
        /** @returns The scroll delta of the mouse since last frame. */
        const DirectX::XMFLOAT2& GetScrollDelta() const;

    protected:
        /**
        * @brief Changes the press-state of a button to true.
        * @param[in] button The button to be changed.
        */
        void SetMouseButtonPressed(MouseButton button);

        /**
        * @brief Changes the release-state of a button to true.
        * @param[in] button The button to be changed.
        */
        void SetMouseButtonReleased(MouseButton button);

        /**
        * @brief Changes the key modifiers of a button to true.
        * @param[in] button The button to be changed.
        * @param[in] modifiers The modifiers that were pressed when the button was pressed.
        */
        void SetMouseButtonModifiers(MouseButton button, int modifiers);

        /**
        * @brief Sets the mouse's position.
        * @param[in] new_mouse_position The mouse's new position.
        */
        void SetMousePosition(const DirectX::XMFLOAT2& new_mouse_position);

        /**
        * @brief Sets the mouse's scroll delta.
        * @param[in] delta_scroll The delta scroll.
        */
        void SetScrollDelta(const DirectX::XMFLOAT2& delta_scroll);

        /**
        * @brief Sets whether the mouse is in the window or not.
        * @param[in] is_mouse_in_window Whether the mouse is in the window or not.
        */
        void SetMouseInWindow(bool is_mouse_in_window);

        /** @brief Updates the MouseState. This makes it so that all accumulators are cleared and mouse states are updated accordingly. */
        void Update();

    private:
        DirectX::XMFLOAT2 mouse_position_;                          //!< The current position of the mouse.
        DirectX::XMFLOAT2 delta_mouse_position_;                    //!< The delta of the mouse position over the last frame.
        DirectX::XMFLOAT2 delta_mouse_position_accumulator_;        //!< An accumulator of delta mouse positions.
        
        DirectX::XMFLOAT2 delta_scroll_;                            //!< The delta scroll of the mouse over the last frame.
        DirectX::XMFLOAT2 delta_scroll_accumulator_;                //!< An accumulator of delta scrolls.

        bool is_mouse_in_window_;                                   //!< Whether the mouse is in the window.

        Map<MouseButton, MouseButtonState> mouse_button_states_;    //!< All the different mouse states.
    };
}