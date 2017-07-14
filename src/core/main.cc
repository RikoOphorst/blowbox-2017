#include <iostream>

#include "core/core/blowbox_core.h"
#include "core/core/blowbox_config.h"
#include "core/get.h"
#include "win32/window.h"
#include "content/image.h"
#include "util/safe_ptr.h"
#include "util/shared_ptr.h"
#include "util/unique_ptr.h"
#include "util/intrusive_ptr.h"
#include "util/linked_ptr.h"
#include "core/scene/scene_manager.h"
#include "core/scene/entity_factory.h"
#include "core/scene/entity.h"
#include "renderer/imgui/imgui.h"

using namespace blowbox;

BlowboxCore* blowbox_instance;
Window* main_window;

bool show_test_window = true;

SharedPtr<Entity> root_child_entity;
SharedPtr<Entity> some_entity;
SharedPtr<Entity> some_child_entity;

double previous_time = 0;

void Run()
{
    main_window = Get::MainWindow().get();

    root_child_entity = EntityFactory::CreateEntity("Entity1");
    EntityFactory::AddChildToParent(Get::SceneManager()->GetRootEntity(), root_child_entity);

    some_entity = EntityFactory::CreateEntity("Entity2");
    some_child_entity = EntityFactory::CreateEntity("Entity3");
    EntityFactory::AddChildToParent(some_entity, some_child_entity);
}

void ImGuiSceneGraph(SharedPtr<Entity> entity)
{
    if (ImGui::TreeNode(entity->GetName().c_str()))
    {
        for (int i = 0; i < entity->GetChildren().size(); i++)
        {
            ImGuiSceneGraph(entity->GetChildren()[i]);
        }

        ImGui::TreePop();
    }
}

void Update()
{
    double delta_time = glfwGetTime() - previous_time;
    previous_time = glfwGetTime();

    ImGui::Begin("SceneGraph");

    ImGuiSceneGraph(Get::SceneManager()->GetRootEntity());

    ImGui::Text("All Entities in SceneManager:");

    Vector<SharedPtr<Entity>>& entities = Get::SceneManager()->GetEntities();
    for (int i = 0; i < entities.size(); i++)
    {
        ImGui::BulletText(entities[i]->GetName().c_str());
    }

    ImGui::Text("Delta time: %f seconds", delta_time);
    ImGui::Text("FPS: %f", 1.0 / delta_time);

    ImGui::End();

    if (Get::MainWindow()->GetKeyboardState().GetKeyPressed(KeyCode_J))
    {
        EntityFactory::AddChildToParent(Get::SceneManager()->GetRootEntity(), some_entity);
    }

    if (Get::MainWindow()->GetKeyboardState().GetKeyPressed(KeyCode_K))
    {
        EntityFactory::RemoveChildFromParent(Get::SceneManager()->GetRootEntity(), some_entity);
    }
}

void PostUpdate()
{
    
}

void Render()
{
    
}

void PostRender()
{
    
}

void Shutdown()
{
    
}

int main(int argc, char** argv)
{
    BlowboxConfig config;
    config.window_title = "Blowbox 2017";
    config.window_resolution = Resolution(1280, 720);

    blowbox_instance = new BlowboxCore(&config);

    blowbox_instance->SetRunProcedure(Run);
    blowbox_instance->SetUpdateProcedure(Update);
    blowbox_instance->SetPostUpdateProcedure(PostUpdate);
    blowbox_instance->SetRenderProcedure(Render);
    blowbox_instance->SetPostRenderProcedure(PostRender);
    blowbox_instance->SetShutdownProcedure(Shutdown);

    blowbox_instance->Run();

    delete blowbox_instance;

    return 0;
}