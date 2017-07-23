#pragma once

#include "core/debug/debug_window.h"
#include "renderer/imgui/imgui.h"
#include "util/shared_ptr.h"
#include "util/weak_ptr.h"

namespace blowbox
{
    class Material;

    class MaterialViewer : public DebugWindow
    {
    public:
        MaterialViewer(WeakPtr<Material> material);
        ~MaterialViewer();

        /** @brief Starts a new frame in the SceneViewer. */
        void NewFrame() override;

        /** @brief Renders the context menu for this window. */
        void RenderMenu() override;

        /** @brief Renders the actual window. */
        void RenderWindow() override;

        /** @returns Whether the viewer window is still shown. */
        bool IsShown() const;

        /** @returns The Entity that is being viewed by this EntityViewer. */
        WeakPtr<Material> GetMaterial() const;

        /** @brief Gives this EntityViewer focus. */
        void Focus();
    private:
        bool show_window_;
        WeakPtr<Material> material_;
        bool focus_;
    };
}