#include <iostream>

#include "core/core/blowbox_core.h"
#include "core/core/blowbox_config.h"
#include "core/get.h"
#include "win32/window.h"
#include "win32/time.h"
#include "content/image.h"
#include "content/model_factory.h"
#include "util/safe_ptr.h"
#include "util/shared_ptr.h"
#include "util/unique_ptr.h"
#include "util/intrusive_ptr.h"
#include "util/linked_ptr.h"
#include "core/scene/scene_manager.h"
#include "core/scene/entity_factory.h"
#include "core/scene/entity.h"
#include "core/debug/console.h"
#include "core/debug/performance_profiler.h"
#include "renderer/imgui/imgui.h"
#include "renderer/device.h"
#include "renderer/buffers/structured_buffer.h"
#include "renderer/descriptor_heap.h"
#include "renderer/cameras/perspective_camera.h"
#include "renderer/cameras/orthographic_camera.h"
#include "renderer/lights/point_light.h"
#include <algorithm>

using namespace blowbox;

BlowboxCore* blowbox_instance;
Window* main_window;

SharedPtr<Entity> my_model;

double previous_time = 0;
bool my_window_open = true;
bool show_test_window = true;

SharedPtr<PointLight> point_lights[128];

void Run()
{
    main_window = Get::MainWindow().get();

    my_model = ModelFactory::LoadModel("./models/crytek-sponza/sponza.obj");
    my_model->SetLocalScaling(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
    EntityFactory::AddChildToEntity(Get::SceneManager()->GetRootEntity(), my_model);

    SharedPtr<PerspectiveCamera> camera = eastl::make_shared<PerspectiveCamera>();

    camera->SetPosition(DirectX::XMFLOAT3(0.0f, 5.0f, 20.0f));
    camera->SetRotation(DirectX::XMFLOAT3(0.0f, 3.14f, 0.0f));
    camera->SetNearPlane(0.1f);
    camera->SetFarPlane(2500.0f);
    camera->SetFovDegrees(90.0f);

    Get::SceneManager()->SetMainCamera(camera);

    for (int i = 0; i < 128; i++)
    {
        point_lights[i] = eastl::make_shared<PointLight>();

        point_lights[i]->SetPosition(DirectX::XMFLOAT3(-174.0f + (rand() % 348), 5.0f + (rand() % 100), -100.0f + (rand() % 200)));
        point_lights[i]->SetColor(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
        point_lights[i]->SetRange(75.0f);
        point_lights[i]->SetIntensity(0.5f);

        Get::SceneManager()->AddPointLight(point_lights[i]);
    }
}

void Update()
{
    SharedPtr<Camera> camera = Get::SceneManager()->GetMainCamera();

    KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

    float speed = 5.0f;

    if (keyboard.GetKeyDown(KeyCode_LEFT_SHIFT))
        speed *= 10.0f;

    if (keyboard.GetKeyDown(KeyCode_W))
        camera->Translate(DirectX::XMFLOAT3(0.0f, 0.0f, speed * Time::GetDeltaTime()));

    if (keyboard.GetKeyDown(KeyCode_S))
        camera->Translate(DirectX::XMFLOAT3(0.0f, 0.0f, -speed * Time::GetDeltaTime()));

    if (keyboard.GetKeyDown(KeyCode_A))
        camera->Translate(DirectX::XMFLOAT3(-speed * Time::GetDeltaTime(), 0.0f, 0.0f));

    if (keyboard.GetKeyDown(KeyCode_D))
        camera->Translate(DirectX::XMFLOAT3(speed * Time::GetDeltaTime(), 0.0f, 0.0f));

    MouseState& mouse = Get::MainWindow()->GetMouseState();
    
    static DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

    if (mouse.GetButtonDown(MouseButton_LEFT))
        camera->Rotate(DirectX::XMFLOAT3(mouse.GetMousePositionDelta().y * 0.005f, mouse.GetMousePositionDelta().x * 0.005f, 0.0f));

    if (keyboard.GetKeyDown(KeyCode_T))
        Get::Console()->LogStatus("status message");
    if (keyboard.GetKeyDown(KeyCode_Y))
        Get::Console()->LogWarning("warning message");
    if (keyboard.GetKeyDown(KeyCode_U))
        Get::Console()->LogError("error message");

    if (keyboard.GetKeyPressed(KeyCode_ENTER))
    {
        Get::Console()->LogStatus(eastl::to_string(camera->GetPosition().x) + " | " + eastl::to_string(camera->GetPosition().y) + " | " + eastl::to_string(camera->GetPosition().z));
    }

    static float light_speed = 50.0f;
    static float light_size = 75.0f;
    static DirectX::XMFLOAT3 light_color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 128; i++)
    {
        DirectX::XMFLOAT3 position = point_lights[i]->GetPosition();
        point_lights[i]->SetPosition(DirectX::XMFLOAT3(position.x, position.y, position.z + light_speed * Time::GetDeltaTime()));
    
        point_lights[i]->SetRange(light_size);
        point_lights[i]->SetColor(light_color);

        if (position.z + light_speed * Time::GetDeltaTime() > 100.0f)
        {
            point_lights[i]->SetPosition(DirectX::XMFLOAT3(-174.0f + (rand() % 348), 5.0f + (rand() % 100), -100.0f + (rand() % 200)));
        }
    }

    ImGui::Begin("Light Sim");
    ImGui::SliderFloat("Light speed", &light_speed, 0.0f, 200.0f);
    ImGui::ColorEdit3("Light color", &light_color.x);
    ImGui::SliderFloat("Light size", &light_size, 1.0f, 250.0f);
    ImGui::End();
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