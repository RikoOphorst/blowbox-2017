#pragma once

#include "core/debug/debug_window.h"
#include "renderer/imgui/imgui.h"
#include "util/shared_ptr.h"
#include "util/unique_ptr.h"
#include "util/vector.h"
#include "util/map.h"

namespace blowbox
{
    class Entity;
    class EntityViewer;

    /** @brief Debug Window for viewing everything that exists in the scene (Entities, Lights, Cameras, etc). */
    class SceneViewer : public DebugWindow
    {
    public:
        /** @brief The view type of the entities being rendered in the entity overview. */
        enum ViewType
        {
            ViewType_LIST,
            ViewType_GRAPH
        };

        SceneViewer();
        ~SceneViewer();

        /** @brief Starts a new frame in the SceneViewer. */
        void NewFrame() override;

        /** @brief Renders the context menu for this window. */
        void RenderMenu() override;

        /** @brief Renders the actual window. */
        void RenderWindow() override;

    protected:
        /**
        * @brief Recursively iterates over an Entity's graph of children.
        * @param[in] entity The entity to iterate over.
        */
        void RenderEntityInGraph(SharedPtr<Entity> entity);

    private:
        bool show_window_;                                          //!< Whether the main window is being shown.
        bool show_entity_view_;                                     //!< Whether the entity overview is being shown.

        ImGuiTextFilter entity_name_filter_;                        //!< Text filter for filtering entities by name.
        ViewType view_type_;                                        //!< The way the entities are rendered in the entity overview.
        
        Map<uintptr_t, UniquePtr<EntityViewer>> entity_viewers_;    //!< All the different active EntityViewer instances.
    };
}