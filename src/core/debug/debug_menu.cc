#include "debug_menu.h"

#include "renderer/imgui/imgui_manager.h"
#include "core/debug/frame_stats.h"
#include "core/debug/memory_stats.h"
#include "core/debug/scene_viewer.h"
#include "core/debug/material_list.h"

#include "util/sort.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    DebugMenu::DebugMenu()
    {

    }

    //------------------------------------------------------------------------------------------------------
    DebugMenu::~DebugMenu()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void DebugMenu::Startup()
    {
        AddDebugWindow(2, eastl::make_shared<FrameStats>(), "FrameStats");
        AddDebugWindow(3, eastl::make_shared<MemoryStats>(), "MemoryStats");
        AddDebugWindow(6, eastl::make_shared<SceneViewer>(), "SceneViewer");
        AddDebugWindow(7, eastl::make_shared<MaterialList>(), "MaterialList");
    }

    //------------------------------------------------------------------------------------------------------
    void DebugMenu::NewFrame()
    {
        for (int i = 0; i < debug_windows_.size(); i++)
        {
            debug_windows_[i].second->NewFrame();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void DebugMenu::Shutdown()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void DebugMenu::RenderMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            for (int i = 0; i < debug_windows_.size(); i++)
            {
                debug_windows_[i].second->RenderMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void DebugMenu::RenderWindows()
    {
        for (int i = 0; i < debug_windows_.size(); i++)
        {
            debug_windows_[i].second->RenderWindow();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void DebugMenu::AddDebugWindow(int priority, SharedPtr<DebugWindow> debug_window, const String& name)
    {
        debug_windows_.push_back(eastl::make_pair(priority, debug_window));

        eastl::quick_sort<Vector<Pair<int, SharedPtr<DebugWindow>>>::iterator, CompareDebugWindowPriority>(debug_windows_.begin(), debug_windows_.end(), CompareDebugWindowPriority());

        if (name != "" && named_windows_.find(name) == named_windows_.end())
        {
            named_windows_[name] = debug_window;
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    SharedPtr<DebugWindow> DebugMenu::GetDebugWindow(const String& name)
    {
        return named_windows_[name];
    }
}