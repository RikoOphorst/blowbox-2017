#pragma once

#include "core/debug/debug_window.h"
#include "renderer/imgui/imgui.h"
#include "util/shared_ptr.h"
#include "util/unique_ptr.h"
#include "util/weak_ptr.h"
#include "util/vector.h"
#include "util/map.h"

namespace blowbox
{
    class Material;
    class MaterialViewer;

    class MaterialList : public DebugWindow
    {
    public:
        MaterialList();
        ~MaterialList();

        /** @brief Starts a new frame in the MaterialList. */
        void NewFrame() override;

        /** @brief Renders the context menu for this window. */
        void RenderMenu() override;

        /** @brief Renders the actual window. */
        void RenderWindow() override;

        void SpawnMaterialViewer(WeakPtr<Material> material);

    private:
        bool show_window_;

        ImGuiTextFilter material_name_filter_;

        Map<uintptr_t, UniquePtr<MaterialViewer>> material_viewers_;
    };
}