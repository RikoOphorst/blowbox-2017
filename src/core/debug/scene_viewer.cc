#include "scene_viewer.h"

#include "core/get.h"
#include "win32/window.h"
#include "core/scene/scene_manager.h"
#include "core/debug/entity_viewer.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    SceneViewer::SceneViewer() :
        show_window_(false),
        view_type_(ViewType_GRAPH)
    {

    }

    //------------------------------------------------------------------------------------------------------
    SceneViewer::~SceneViewer()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void SceneViewer::NewFrame()
    {
        for (auto it = entity_viewers_.rbegin(); it != entity_viewers_.rend(); it++)
        {
            if (it->second->IsShown())
            {
                it->second->NewFrame();
            }
            else
            {
                entity_viewers_.erase(it);

                if (entity_viewers_.size() == 0)
                {
                    break;
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneViewer::RenderMenu()
    {
        if (ImGui::BeginMenu("Scene Viewer"))
        {
            if (!show_window_)
            {
                if (ImGui::MenuItem("Show Scene Viewer", "CTRL+6", false, !show_window_))
                {
                    show_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Scene Viewer", "CTRL+6", false, show_window_))
                {
                    show_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_6))
        {
            show_window_ = !show_window_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneViewer::RenderWindow()
    {
        if (show_window_)
        {
            ImGui::SetNextWindowSize(ImVec2(450.0f, 300.0f), ImGuiSetCond_Once);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_Once);
            if (ImGui::Begin("Scene Viewer", &show_window_))
            {
                int view_type = static_cast<int>(view_type_);
                ImGui::RadioButton("View as list", &view_type, 0);
                ImGui::SameLine();
                ImGui::RadioButton("View as graph", &view_type, 1);
                view_type_ = static_cast<ViewType>(view_type);

                ImGui::Separator();

                entity_name_filter_.Draw("Search for entities");

                if (view_type_ == ViewType_LIST || entity_name_filter_.InputBuf[0] != '\0')
                {
                    Vector<SharedPtr<Entity>>& entities = Get::SceneManager()->GetEntities();
                    for (int i = 0; i < entities.size(); i++)
                    {
                        if (entity_name_filter_.PassFilter(entities[i]->GetName().c_str()))
                        {
                            ImGui::Text("%i:\t", i);
                            ImGui::SameLine();
                            if (ImGui::Selectable(entities[i]->GetName().c_str()))
                            {
                                if (entity_viewers_.find(reinterpret_cast<uintptr_t>(entities[i].get())) == entity_viewers_.end())
                                {
                                    entity_viewers_[reinterpret_cast<uintptr_t>(entities[i].get())] = eastl::make_unique<EntityViewer>(entities[i]);
                                }
                                else
                                {
                                    entity_viewers_[reinterpret_cast<uintptr_t>(entities[i].get())]->Focus();
                                }
                            }
                        }
                    }
                }
                else
                {
                    RenderEntityInGraph(Get::SceneManager()->GetRootEntity());
                }

                ImGui::End();
            }
        }

        for (auto it = entity_viewers_.begin(); it != entity_viewers_.end(); it++)
        {
            it->second->RenderWindow();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneViewer::RenderEntityInGraph(SharedPtr<Entity> entity)
    {
        if (ImGui::TreeNode(entity->GetName().c_str()))
        {
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
            if (ImGui::Selectable("(Click to open in viewer)", entity_viewers_.find(reinterpret_cast<uintptr_t>(entity.get())) != entity_viewers_.end()))
            {
                if (entity_viewers_.find(reinterpret_cast<uintptr_t>(entity.get())) == entity_viewers_.end())
                {
                    entity_viewers_[reinterpret_cast<uintptr_t>(entity.get())] = eastl::make_unique<EntityViewer>(entity);
                }
                else
                {
                    entity_viewers_[reinterpret_cast<uintptr_t>(entity.get())]->Focus();
                }
            }
            ImGui::PopStyleColor();

            Vector<SharedPtr<Entity>> children = entity->GetChildren();
            for (int i = 0; i < children.size(); i++)
            {
                RenderEntityInGraph(children[i]);
            }

            ImGui::TreePop();
        }
    }
}