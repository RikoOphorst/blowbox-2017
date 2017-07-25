#include "mouse_state.h"

#include <GLFW/glfw3.h>
#include "core/debug/performance_profiler.h"
#include "renderer/imgui/imgui.h"

namespace blowbox
{
    MouseState::MouseButtonState MouseState::default_mouse_button_state_ = { false, false, false, false, false, false };
    DirectX::XMFLOAT2 MouseState::default_mouse_float2_ = DirectX::XMFLOAT2(0.0f, 0.0f);

    //------------------------------------------------------------------------------------------------------
    MouseState::MouseState() :
        mouse_position_(0.0f, 0.0f),
        delta_mouse_position_(0.0f, 0.0f),
        delta_mouse_position_accumulator_(0.0f, 0.0f),
        delta_scroll_(0.0f, 0.0f),
        delta_scroll_accumulator_(0.0f, 0.0f),
        is_mouse_in_window_(false)
    {
        for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
        {
            mouse_button_states_[static_cast<MouseButton>(i)] = MouseState::MouseButtonState();
        }
    }

    //------------------------------------------------------------------------------------------------------
    MouseState::~MouseState()
    {

    }

    //------------------------------------------------------------------------------------------------------
    bool MouseState::GetButton(MouseButton button, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureMouse)
        {
            return mouse_button_states_[button].down;
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    bool MouseState::GetButtonDown(MouseButton button, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureMouse)
        {
            return mouse_button_states_[button].down;
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    bool MouseState::GetButtonPressed(MouseButton button, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureMouse)
        {
            return mouse_button_states_[button].pressed;
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    bool MouseState::GetButtonReleased(MouseButton button, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureMouse)
        {
            return mouse_button_states_[button].released;
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT2& MouseState::GetMousePosition(bool imgui) const
    {
        if (imgui || !ImGui::GetIO().WantCaptureMouse)
        {
            return mouse_position_;
        }
        else
        {
            return default_mouse_float2_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT2& MouseState::GetMousePositionDelta(bool imgui) const
    {
        if (imgui || !ImGui::GetIO().WantCaptureMouse)
        {
            return delta_mouse_position_;
        }
        else
        {
            return default_mouse_float2_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT2& MouseState::GetScrollDelta(bool imgui) const
    {
        if (imgui || !ImGui::GetIO().WantCaptureMouse)
        {
            return delta_scroll_;
        }
        else
        {
            return default_mouse_float2_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MouseState::SetMouseButtonPressed(MouseButton button)
    {
        mouse_button_states_[button].down = true;
        mouse_button_states_[button].pressed = true;
    }

    //------------------------------------------------------------------------------------------------------
    void MouseState::SetMouseButtonReleased(MouseButton button)
    {
        mouse_button_states_[button].down = false;
        mouse_button_states_[button].released = true;
    }

    //------------------------------------------------------------------------------------------------------
    void MouseState::SetMouseButtonModifiers(MouseButton button, int modifiers)
    {
        mouse_button_states_[button].modifier_alt       = (modifiers & GLFW_MOD_ALT)        == GLFW_MOD_ALT;
        mouse_button_states_[button].modifier_control   = (modifiers & GLFW_MOD_CONTROL)    == GLFW_MOD_CONTROL;
        mouse_button_states_[button].modifier_shift     = (modifiers & GLFW_MOD_SHIFT)      == GLFW_MOD_SHIFT;
    }

    //------------------------------------------------------------------------------------------------------
    void MouseState::SetMousePosition(const DirectX::XMFLOAT2& new_mouse_position)
    {
        DirectX::XMFLOAT2 this_mouse_position_accumulator = DirectX::XMFLOAT2(
            new_mouse_position.x - mouse_position_.x,
            new_mouse_position.y - mouse_position_.y
        );

        delta_mouse_position_accumulator_ = DirectX::XMFLOAT2(
            delta_mouse_position_accumulator_.x + this_mouse_position_accumulator.x,
            delta_mouse_position_accumulator_.y + this_mouse_position_accumulator.y
        );

        mouse_position_ = new_mouse_position;
    }

    //------------------------------------------------------------------------------------------------------
    void MouseState::SetScrollDelta(const DirectX::XMFLOAT2& delta_scroll)
    {
        delta_scroll_accumulator_ = DirectX::XMFLOAT2(
            delta_scroll_accumulator_.x + delta_scroll.x,
            delta_scroll_accumulator_.y + delta_scroll.y
        );
    }

    //------------------------------------------------------------------------------------------------------
    void MouseState::SetMouseInWindow(bool is_mouse_in_window)
    {
        is_mouse_in_window_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void MouseState::Update()
    {
        PerformanceProfiler::ProfilerBlock block("KeyboardState::Update", ProfilerBlockType_CORE);
        for (auto it = mouse_button_states_.begin(); it != mouse_button_states_.end(); it++)
        {
            it->second.pressed = false;
            it->second.released = false;
        }

        delta_mouse_position_ = delta_mouse_position_accumulator_;
        delta_scroll_ = delta_scroll_accumulator_;

        delta_mouse_position_accumulator_ = DirectX::XMFLOAT2(0.0f, 0.0f);
        delta_scroll_accumulator_ = DirectX::XMFLOAT2(0.0f, 0.0f);
    }
}