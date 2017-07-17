#pragma once

namespace blowbox
{
    /**
    * @brief Base class for DebugWindows.
    */
    class DebugWindow
    {
    public:
        DebugWindow();
        virtual ~DebugWindow();

        /** @brief Renders the actual window. */
        virtual void RenderWindow() = 0;

        /** @brief Renders the context menu for this window. */
        virtual void RenderMenu() = 0;
    };
}