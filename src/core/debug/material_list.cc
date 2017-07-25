#include "material_list.h"

#include "core/get.h"
#include "core/debug/material_viewer.h"
#include "renderer/materials/material.h"
#include "renderer/materials/material_manager.h"
#include "win32/window.h"
#include "util/sort.h"
#include "util/algorithm.h"

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
            ImGui::SetNextWindowSize(ImVec2(300.0f, 450.0f), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);

            if (ImGui::Begin("Material List", &show_window_))
            {
                UnorderedMap<String, SharedPtr<Material>> materials = Get::MaterialManager()->materials_;

                material_name_filter_.Draw("Find materials", 170.0f);

                ImGui::Separator();

                Vector<SharedPtr<Material>> sorted_materials;

                for (auto it = materials.begin(); it != materials.end(); it++)
                {
                    sorted_materials.push_back(it->second);
                }
                
                auto compare = [](const SharedPtr<Material>& a, const SharedPtr<Material>& b)
                {
                    String name1 = a->GetName();
                    String name2 = b->GetName();
                    eastl::transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
                    eastl::transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
                    return name1 < name2;
                };

                eastl::sort(sorted_materials.begin(), sorted_materials.end(), compare);

                for (int i = 0; i < sorted_materials.size(); i++)
                {
                    if (material_name_filter_.PassFilter(sorted_materials[i]->GetName().c_str()))
                    {
                        ImGui::Text("%i:", i);
                        ImGui::SameLine(50.0f);
                        if (ImGui::Selectable(sorted_materials[i]->GetName().c_str(), material_viewers_.find(reinterpret_cast<uintptr_t>(sorted_materials[i].get())) != material_viewers_.end()))
                        {
                            SpawnMaterialViewer(sorted_materials[i]);
                        }
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