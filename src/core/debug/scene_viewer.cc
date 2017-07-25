#include "scene_viewer.h"

#include "core/get.h"
#include "win32/window.h"
#include "core/scene/scene_manager.h"
#include "core/debug/entity_viewer.h"
#include "util/sort.h"
#include "util/algorithm.h"
#include "renderer/cameras/orthographic_camera.h"
#include "renderer/cameras/perspective_camera.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    SceneViewer::SceneViewer() :
        show_window_(false),
        view_type_(ViewType_GRAPH),
        show_entity_view_(true)
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
            ImGui::SetNextWindowSize(ImVec2(450.0f, 300.0f), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
            if (ImGui::Begin("Scene Viewer", &show_window_))
            {
                if (ImGui::CollapsingHeader("Entities", &show_entity_view_))
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
                        Vector<SharedPtr<Entity>> entities = Get::SceneManager()->GetEntities();

                        auto compare = [](const SharedPtr<Entity>& a, const SharedPtr<Entity>& b)
                        {
                            String name1 = a->GetName();
                            String name2 = b->GetName();
                            eastl::transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
                            eastl::transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
                            return name1 < name2;
                        };

                        eastl::sort(entities.begin(), entities.end(), compare);

                        for (int i = 0; i < entities.size(); i++)
                        {
                            if (entity_name_filter_.PassFilter(entities[i]->GetName().c_str()))
                            {
                                ImGui::Text("%i:", i);
                                ImGui::SameLine(50.0f);
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
                }

                if (ImGui::CollapsingHeader("Main Camera"))
                {
                    SharedPtr<Camera> base_camera = Get::SceneManager()->GetMainCamera();

                    DirectX::XMFLOAT3 temp_float3 = base_camera->GetPosition();
                    if (ImGui::InputFloat3("Position", &temp_float3.x, 2))
                    {
                        base_camera->SetPosition(temp_float3);
                    }

                    temp_float3 = base_camera->GetRotation();
                    if (ImGui::InputFloat3("Rotation", &temp_float3.x, 2))
                    {
                        base_camera->SetRotation(temp_float3);
                    }

                    float temp_float = base_camera->GetNearPlane();
                    if (ImGui::InputFloat("Near Plane", &temp_float, 0.0f, 0.0f, 2))
                    {
                        base_camera->SetNearPlane(eastl::clamp(temp_float, 0.01f, base_camera->GetFarPlane() - 1.0f));
                    }

                    temp_float = base_camera->GetFarPlane();
                    if (ImGui::InputFloat("Far Plane", &temp_float, 0.0f, 0.0f, 2))
                    {
                        base_camera->SetFarPlane(eastl::clamp(temp_float, base_camera->GetNearPlane() + 1.0f, 50000.0f));
                    }

                    if (dynamic_cast<PerspectiveCamera*>(base_camera.get()) != nullptr)
                    {
                        PerspectiveCamera* camera = dynamic_cast<PerspectiveCamera*>(base_camera.get());

                        temp_float = camera->GetFovDegrees();
                        if (ImGui::InputFloat("FOV (Degrees)", &temp_float, 0.0f, 0.0f, 2))
                        {
                            camera->SetFovDegrees(temp_float);
                        }

                        if (ImGui::Button("Switch to Orthographic"))
                        {
                            SharedPtr<OrthographicCamera> new_camera = eastl::make_shared<OrthographicCamera>();
                            new_camera->SetPosition(camera->GetPosition());
                            new_camera->SetRotation(camera->GetRotation());
                            new_camera->SetNearPlane(camera->GetNearPlane());
                            new_camera->SetFarPlane(camera->GetFarPlane());
                            new_camera->SetOrthographicSize(1.0f);

                            Get::SceneManager()->SetMainCamera(new_camera);
                        }
                    }
                    else
                    {
                        OrthographicCamera* camera = dynamic_cast<OrthographicCamera*>(base_camera.get());

                        temp_float = camera->GetOrthographicSize();
                        if (ImGui::InputFloat("Orthographic Size", &temp_float, 0.0f, 0.0f, 2))
                        {
                            camera->SetOrthographicSize(temp_float);
                        }

                        if (ImGui::Button("Switch to Perspective"))
                        {
                            SharedPtr<PerspectiveCamera> new_camera = eastl::make_shared<PerspectiveCamera>();
                            new_camera->SetPosition(camera->GetPosition());
                            new_camera->SetRotation(camera->GetRotation());
                            new_camera->SetNearPlane(camera->GetNearPlane());
                            new_camera->SetFarPlane(camera->GetFarPlane());
                            new_camera->SetFovDegrees(90.0f);

                            Get::SceneManager()->SetMainCamera(new_camera);
                        }
                    }
                }
            }

            ImGui::End();
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