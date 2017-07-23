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

    class SceneViewer : public DebugWindow
    {
    public:
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
        void RenderEntityInGraph(SharedPtr<Entity> entity);

    private:
        bool show_window_;

        ImGuiTextFilter entity_name_filter_;
        ViewType view_type_;
        
        Map<uintptr_t, UniquePtr<EntityViewer>> entity_viewers_;
    };
}