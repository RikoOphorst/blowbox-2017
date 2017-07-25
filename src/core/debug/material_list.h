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

    /** @brief Lists all Materials that are currently in the MaterialManager. */
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

        /**
        * @brief Spawns a MaterialViewer.
        * @param[in] material The Material the new MaterialViewer should be based on.
        */
        void SpawnMaterialViewer(WeakPtr<Material> material);

    private:
        bool show_window_;                                              //!< Whether the main window is being shown.
        ImGuiTextFilter material_name_filter_;                          //!< Text filter for searching for materials in the list. 
        Map<uintptr_t, UniquePtr<MaterialViewer>> material_viewers_;    //!< All material viewers active in this MaterialList.
    };
}