#include "material_list.h"

#include "core/get.h"
#include "core/debug/material_viewer.h"
#include "renderer/material.h"
#include "renderer/material_manager.h"
#include "win32/window.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    MaterialList::MaterialList() :
        show_window_(false)
    {

    }

    //------------------------------------------------------------------------------------------------------
    MaterialList::~MaterialList()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MaterialList::NewFrame()
    {
        for (auto it = material_viewers_.rbegin(); it != material_viewers_.rend(); it++)
        {
            if (it->second->IsShown())
            {
                it->second->NewFrame();
            }
            else
            {
                material_viewers_.erase(it);

                if (material_viewers_.size() == 0)
                {
                    break;
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MaterialList::RenderMenu()
    {
        if (ImGui::BeginMenu("Material List"))
        {
            if (!show_window_)
            {
                if (ImGui::MenuItem("Show Material List", "CTRL+7", false, !show_window_))
                {
                    show_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Material List", "CTRL+7", false, show_window_))
                {
                    show_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_7))
        {
            show_window_ = !show_window_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MaterialList::RenderWindow()
    {
        if (show_window_)
        {
            ImGui::SetNextWindowSize(ImVec2(450.0f, 300.0f), ImGuiSetCond_Once);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_Once);

            if (ImGui::Begin("Material List", &show_window_))
            {
                UnorderedMap<String, SharedPtr<Material>> materials = Get::MaterialManager()->materials_;

                for (auto it = materials.begin(); it != materials.end(); it++)
                {
                    if (ImGui::Selectable(it->first.c_str()))
                    {
                        SpawnMaterialViewer(it->second);
                    }
                }
            }

            ImGui::End();
        }

        for (auto it = material_viewers_.begin(); it != material_viewers_.end(); it++)
        {
            it->second->RenderWindow();
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    void MaterialList::SpawnMaterialViewer(WeakPtr<Material> material)
    {
        if (!material.expired())
        {
            uintptr_t ptr = reinterpret_cast<uintptr_t>(material.lock().get());
            auto it = material_viewers_.find(ptr);

            if (it == material_viewers_.end())
            {
                material_viewers_[ptr] = eastl::make_unique<MaterialViewer>(material);
            }
        }
    }
}