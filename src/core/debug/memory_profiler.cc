#include "memory_profiler.h"

#include "core/get.h"
#include "win32/window.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    MemoryProfiler::MemoryProfiler()
    {

    }

    //------------------------------------------------------------------------------------------------------
    MemoryProfiler::~MemoryProfiler()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::RenderMenu()
    {
        if (ImGui::BeginMenu("Memory Profiler"))
        {
            if (!show_window_)
            {
                if (ImGui::MenuItem("Show Memory Profiler", "CTRL+5", false, !show_window_))
                {
                    show_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Memory Profiler", "CTRL+5", false, show_window_))
                {
                    show_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_5))
        {
            show_window_ = !show_window_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::RenderWindow()
    {
        if (show_window_)
        {
            if (ImGui::Begin("Memory Profiler", &show_window_))
            {
                ImGui::End();
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::NewFrame()
    {

    }
}