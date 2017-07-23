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
#include <algorithm>

#include <Psapi.h>

using namespace blowbox;

BlowboxCore* blowbox_instance;
Window* main_window;

SharedPtr<Entity> my_model;
SharedPtr<PerspectiveCamera> camera;

double previous_time = 0;
bool my_window_open = true;
bool show_test_window = true;

void Run()
{
    main_window = Get::MainWindow().get();

    my_model = ModelFactory::LoadModel("./models/nanosuit/nanosuit.obj");
    //my_model->SetLocalScaling(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
    EntityFactory::AddChildToEntity(Get::SceneManager()->GetRootEntity(), my_model);

    camera = eastl::make_shared<PerspectiveCamera>();

    camera->SetPosition(DirectX::XMFLOAT3(0.0f, 5.0f, 20.0f));
    camera->SetRotation(DirectX::XMFLOAT3(0.0f, 3.14f, 0.0f));
    camera->SetNearPlane(0.1f);
    camera->SetFarPlane(2500.0f);
    camera->SetFovDegrees(90.0f);

    Get::SceneManager()->SetMainCamera(camera);
}

void Update()
{
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

    //my_model->SetLocalPosition(DirectX::XMFLOAT3(std::sin(Time::GetProcessTime()) * 15.0f, 0.0f, std::cos(Time::GetProcessTime()) * 15.0f));
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