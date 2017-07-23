#pragma once

#include "util/vector.h"
#include "util/map.h"
#include "util/utility.h"
#include "util/shared_ptr.h"
#include "util/string.h"
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

        /** @brief Starts up the DebugMenu. */
        void Startup();

        /** @brief Starts a new frame. */
        void NewFrame();

        /** @brief Shuts down the DebugMenu. */
        void Shutdown();

        /** @brief Renders the entire main menu bar. */
        void RenderMenu();

        /** @brief Renders all DebugWindows that have subscribed to the DebugMenu. */
        void RenderWindows();

        /**
        * @brief Adds a debug window to the DebugMenu.
        * @param[in] priority The priority of the DebugWindow. This is an arbitrary number that you can define yourself. It influences the order in which the debug window menus are rendered.
        * @param[in] debug_window The DebugWindow that should be added.
        * @param[in] name This is optional. Makes the window retrievable via GetDebugWindow().
        */
        void AddDebugWindow(int priority, SharedPtr<DebugWindow> debug_window, const String& name = "");

        /**
        * @brief Retrieve a DebugWindow by its name.
        * @param[in] name The name of the window to be retrieved.
        */
        SharedPtr<DebugWindow> GetDebugWindow(const String& name);
    private:
        Vector<Pair<int, SharedPtr<DebugWindow>>> debug_windows_; //!< All debug windows that have been added to the DebugMenu.
        Map<String, SharedPtr<DebugWindow>> named_windows_; //!< The same as debug_window_, except it isn't prioritized, but links names to debug windows.

    protected:
        /** @brief Compares the priorities for DebugWindows. */
        struct CompareDebugWindowPriority
        {
            /**
            * @brief Compares the priorities for DebugWindows.
            * @param[in] a The first instance.
            * @param[in] b The second instance.
            */
            inline bool operator() (const Pair<int, SharedPtr<DebugWindow>>& a, const Pair<int, SharedPtr<DebugWindow>>& b)
            {
                return (a.first < b.first);
            }
        };
    };
}