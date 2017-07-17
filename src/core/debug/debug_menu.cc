#include "debug_menu.h"

#include "renderer/imgui/imgui_manager.h"

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
    void DebugMenu::RenderMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            for (int i = 0; i < debug_windows_.size(); i++)
            {
                debug_windows_[i].second.lock()->RenderMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void DebugMenu::RenderWindows()
    {
        for (int i = 0; i < debug_windows_.size(); i++)
        {
            debug_windows_[i].second.lock()->RenderWindow();
        }
    }

    struct less_than_key
    {
        inline bool operator() (const Pair<int, WeakPtr<DebugWindow>>& a, const Pair<int, WeakPtr<DebugWindow>>& b)
        {
            return (a.first < b.first);
        }
    };

    //------------------------------------------------------------------------------------------------------
    void DebugMenu::AddDebugWindow(int priority, WeakPtr<DebugWindow> debug_window)
    {
        debug_windows_.push_back(eastl::make_pair(priority, debug_window));

        eastl::quick_sort<Vector<Pair<int, WeakPtr<DebugWindow>>>::iterator, less_than_key>(debug_windows_.begin(), debug_windows_.end(), less_than_key());
    }
}