#pragma once

#include "win32/mouse_button.h"
#include "core/eastl_required.h"
#include <EASTL/map.h>
#include <DirectXMath.h>

namespace blowbox
{
    class MouseState
    {
        friend class Window;
        friend class GLFWManager;
        friend class ImGuiManager;
    public:
        struct MouseButtonState
        {
            bool down = false;
            bool pressed = false;
            bool released = false;
            bool modifier_control = false;
            bool modifier_alt = false;
            bool modifier_shift = false;
        };

        MouseState();
        ~MouseState();

        bool GetButton(MouseButton button);
        bool GetButtonDown(MouseButton button);
        bool GetButtonPressed(MouseButton button);
        bool GetButtonReleased(MouseButton button);

        const DirectX::XMFLOAT2& GetMousePosition() const;
        const DirectX::XMFLOAT2& GetMousePositionDelta() const;
        const DirectX::XMFLOAT2& GetScrollDelta() const;

    protected:
        void SetMouseButtonPressed(MouseButton button);
        void SetMouseButtonReleased(MouseButton button);
        void SetMouseButtonModifiers(MouseButton button, int modifiers);

        void SetMousePosition(const DirectX::XMFLOAT2& new_mouse_position);
        void SetScrollDelta(const DirectX::XMFLOAT2& delta_scroll);
        void SetMouseInWindow(bool is_mouse_in_window);

        void Update();

    private:
        DirectX::XMFLOAT2 mouse_position_;
        DirectX::XMFLOAT2 delta_mouse_position_;
        DirectX::XMFLOAT2 delta_mouse_position_accumulator_;
        
        DirectX::XMFLOAT2 delta_scroll_;
        DirectX::XMFLOAT2 delta_scroll_accumulator_;

        bool is_mouse_in_window_;

        eastl::map<MouseButton, MouseButtonState> mouse_button_states_;
    };
}