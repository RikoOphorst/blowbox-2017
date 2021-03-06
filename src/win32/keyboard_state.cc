#include "keyboard_state.h"

#include <GLFW/glfw3.h>
#include "core/debug/performance_profiler.h"
#include "renderer/imgui/imgui.h"

namespace blowbox
{
    KeyboardState::KeyState KeyboardState::default_key_state_ = { false, false, false, false, false, false };

    //------------------------------------------------------------------------------------------------------
    KeyboardState::KeyboardState()
    {
        for (int i = 0; i < GLFW_KEY_LAST; i++)
        {
            key_states_[static_cast<KeyCode>(i)] = KeyboardState::KeyState();
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    KeyboardState::~KeyboardState()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    bool KeyboardState::GetKey(KeyCode key, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureKeyboard)
        {
            return key_states_[key].down;
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    bool KeyboardState::GetKeyDown(KeyCode key, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureKeyboard)
        {
            return key_states_[key].down;
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    bool KeyboardState::GetKeyPressed(KeyCode key, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureKeyboard)
        {
            return key_states_[key].pressed;
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    bool KeyboardState::GetKeyReleased(KeyCode key, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureKeyboard)
        {
            return key_states_[key].released;
        }
        else
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------------------------------
    const KeyboardState::KeyState& KeyboardState::GetKeyState(KeyCode key, bool imgui)
    {
        if (imgui || !ImGui::GetIO().WantCaptureKeyboard)
        {
            return key_states_[key];
        }
        else
        {
            return default_key_state_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    Queue<unsigned int> KeyboardState::GetInputKeys() const
    {
        return input_keys_;
    }
    
    //------------------------------------------------------------------------------------------------------
    void KeyboardState::SetKeyPressed(KeyCode key)
    {
        key_states_[key].down = true;
        key_states_[key].pressed = true;
    }

    //------------------------------------------------------------------------------------------------------
    void KeyboardState::SetKeyReleased(KeyCode key)
    {
        key_states_[key].down = false;
        key_states_[key].released = true;
    }

    //------------------------------------------------------------------------------------------------------
    void KeyboardState::SetKeyModifiers(KeyCode key, int modifiers)
    {
        key_states_[key].modifier_alt       = (modifiers & GLFW_MOD_ALT)        == GLFW_MOD_ALT;
        key_states_[key].modifier_control   = (modifiers & GLFW_MOD_CONTROL)    == GLFW_MOD_CONTROL;
        key_states_[key].modifier_shift     = (modifiers & GLFW_MOD_SHIFT)      == GLFW_MOD_SHIFT;
    }
    
    //------------------------------------------------------------------------------------------------------
    void KeyboardState::AddInputKey(unsigned int key)
    {
        input_keys_.push(key);
    }

    //------------------------------------------------------------------------------------------------------
    void KeyboardState::ResetKeys()
    {
        PerformanceProfiler::ProfilerBlock block("KeyboardState::Reset", ProfilerBlockType_CORE);

        for (auto it = key_states_.begin(); it != key_states_.end(); it++)
        {
            it->second.pressed = false;
            it->second.released = false;
        }

        while (!input_keys_.empty())
        {
            input_keys_.pop();
        }
    }
}