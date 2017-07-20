#pragma once

#include "util/vector.h"
#include "util/utility.h"
#include "util/weak_ptr.h"
#include "core/debug/debug_window.h"

namespace blowbox
{
    /**
    * DebugMenu manages all debug windows and operates the main menu bar.
    * New DebugWindows should be added to this if they want to appear in 
    * the main menu bar.
    *
    * @brief Manages all debug windows and operates the main menu bar.
    */
    class DebugMenu
    {
    public:
        DebugMenu();
        ~DebugMenu();

        /** @brief Renders the entire main menu bar. */
        void RenderMenu();

        /** @brief Renders all DebugWindows that have subscribed to the DebugMenu. */
        void RenderWindows();

        /**
        * @brief Adds a debug window to the DebugMenu.
        * @param[in] priority The priority of the DebugWindow. This is an arbitrary number that you can define yourself. It influences the order in which the debug window menus are rendered.
        * @param[in] debug_window The DebugWindow that should be added.
        */
        void AddDebugWindow(int priority, WeakPtr<DebugWindow> debug_window);
    private:
        Vector<Pair<int, WeakPtr<DebugWindow>>> debug_windows_; //!< All debug windows that have been added to the DebugMenu.

    protected:
        /** @brief Compares the priorities for DebugWindows. */
        struct CompareDebugWindowPriority
        {
            /**
            * @brief Compares the priorities for DebugWindows.
            * @param[in] a The first instance.
            * @param[in] b The second instance.
            */
            inline bool operator() (const Pair<int, WeakPtr<DebugWindow>>& a, const Pair<int, WeakPtr<DebugWindow>>& b)
            {
                return (a.first < b.first);
            }
        };
    };
}