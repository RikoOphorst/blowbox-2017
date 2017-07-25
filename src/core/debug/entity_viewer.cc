#include "entity_viewer.h"

#include "core/get.h"
#include "core/debug/debug_menu.h"
#include "core/debug/material_list.h"
#include "core/scene/entity.h"
#include "renderer/material.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    EntityViewer::EntityViewer(WeakPtr<Entity> entity) :
        show_window_(true),
        entity_(entity)
    {
        
    }
    
    //------------------------------------------------------------------------------------------------------
    EntityViewer::~EntityViewer()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    void EntityViewer::NewFrame()
    {
        
    }
    
    //------------------------------------------------------------------------------------------------------
    void EntityViewer::RenderMenu()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    void EntityViewer::RenderWindow()
    {
        if (entity_.expired())
        {
            show_window_ = false;
        }

        if (show_window_)
        {
            SharedPtr<Entity> entity = entity_.lock();
            
            char buf[256];
            sprintf(buf, "EntityView: %s###Entity%i", entity->GetName().c_str(), reinterpret_cast<uintptr_t>(entity.get()), reinterpret_cast<uintptr_t>(entity.get()));

            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(275.0f, 300.0f), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowCollapsed(false, ImGuiSetCond_FirstUseEver);

            if (focus_)
            {
                ImGui::SetNextWindowCollapsed(false, ImGuiSetCond_Always);
                ImGui::SetNextWindowFocus();
                focus_ = false;
            }

            if (ImGui::Begin(buf, &show_window_))
            {
                char buf[128];
                sprintf(buf, "%s", entity->GetName().c_str());
                if (ImGui::InputText("Name", buf, 128))
                {
                    entity->SetName(buf);
                }

                ImGui::Separator();

                DirectX::XMFLOAT3 temp = entity->GetLocalPosition();
                if (ImGui::InputFloat3("Position", &temp.x, 2))
                {
                    entity->SetLocalPosition(temp);
                }
                
                temp = entity->GetLocalRotation();
                if (ImGui::InputFloat3("Rotation", &temp.x, 2))
                {
                    entity->SetLocalRotation(temp);
                }
                
                temp = entity->GetLocalScaling();
                if (ImGui::InputFloat3("Scaling", &temp.x, 2))
                {
                    entity->SetLocalScaling(temp);
                }

                ImGui::Separator();

                ImGui::Text("Mesh:");
                ImGui::SameLine(100.0f);
                if (entity->GetMesh() == nullptr)
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    ImGui::Selectable(entity->GetMesh()->GetMeshData().GetName().c_str());
                }

                ImGui::Text("Material:");
                ImGui::SameLine(100.0f);
                if (entity->GetMaterial().expired())
                {
                    ImGui::Selectable("-");
                }
                else
                {
                    if (ImGui::Selectable(entity->GetMaterial().lock()->GetName().c_str()))
                    {
                        SharedPtr<DebugWindow> window = Get::DebugMenu()->GetDebugWindow("MaterialList");
                        MaterialList* material_list = static_cast<MaterialList*>(window.get());

                        material_list->SpawnMaterialViewer(entity->GetMaterial());
                    }
                }

                ImGui::Text("Visible:");
                ImGui::SameLine(100.0f);
                bool temp_bool = entity->GetVisible();
                if (ImGui::Checkbox("", &temp_bool))
                {
                    entity->SetVisible(temp_bool);
                }
            }

            ImGui::End();
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    bool EntityViewer::IsShown() const
    {
        return show_window_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Entity> EntityViewer::GetEntity() const
    {
        return entity_;
    }

    //------------------------------------------------------------------------------------------------------
    void EntityViewer::Focus()
    {
        focus_ = true;
    }
}