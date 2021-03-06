#pragma once

#include "core/debug/debug_window.h"
#include "renderer/imgui/imgui.h"
#include "util/shared_ptr.h"
#include "util/weak_ptr.h"

namespace blowbox
{
    class Entity;

    /** @brief DebugWindow for viewing/editing a specific Entity. */
    class EntityViewer : public DebugWindow
    {
    public:
        /**
        * @brief Constructs an EntityViewer.
        * @param[in] entity The entity to be viewed.
        */
        EntityViewer(WeakPtr<Entity> entity);
        ~EntityViewer();

        /** @brief Starts a new frame in the SceneViewer. */
        void NewFrame() override;

        /** @brief Renders the context menu for this window. */
        void RenderMenu() override;

        /** @brief Renders the actual window. */
        void RenderWindow() override;

        /** @returns Whether the viewer window is still shown. */
        bool IsShown() const;

        /** @returns The Entity that is being viewed by this EntityViewer. */
        WeakPtr<Entity> GetEntity() const;

        /** @brief Gives this EntityViewer focus. */
        void Focus();
    private:
        bool show_window_;      //!< Whether the window is being shown.
        WeakPtr<Entity> entity_;//!< The Entity this EntityViewer is describing.
        bool focus_;            //!< Whether this window wants focus.
    };
}