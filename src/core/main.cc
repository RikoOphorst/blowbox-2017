#include <iostream>

#include "core/core/blowbox_core.h"
#include "core/core/blowbox_config.h"
#include "core/get.h"
#include "win32/window.h"
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
#include "renderer/imgui/imgui.h"
#include "renderer/device.h"
#include "renderer/buffers/structured_buffer.h"
#include "renderer/descriptor_heap.h"
#include "renderer/cameras/perspective_camera.h"
#include "renderer/cameras/orthographic_camera.h"

#include <Psapi.h>

using namespace blowbox;

BlowboxCore* blowbox_instance;
Window* main_window;

SharedPtr<Entity> my_model;
SharedPtr<PerspectiveCamera> camera;

double previous_time = 0;

void Run()
{
    main_window = Get::MainWindow().get();

    my_model = ModelFactory::LoadModel("./models/crytek-sponza/sponza.obj");
    my_model->SetLocalScaling(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
    EntityFactory::AddChildToEntity(Get::SceneManager()->GetRootEntity(), my_model);

    camera = eastl::make_shared<PerspectiveCamera>();

    camera->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, 0.0f));
    camera->SetRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    camera->SetNearPlane(0.1f);
    camera->SetFarPlane(2500.0f);
    camera->SetFovDegrees(90.0f);

    Get::SceneManager()->SetMainCamera(camera);
}

void Update()
{
    double delta_time = glfwGetTime() - previous_time;
    previous_time = glfwGetTime();

    ImGui::Text("Delta time: %f seconds", delta_time);
    ImGui::Text("FPS: %f", 1.0 / delta_time);

    DXGI_QUERY_VIDEO_MEMORY_INFO video_memory_info;
    Get::Device()->GetAdapter().dxgi_adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &video_memory_info);

    ImGui::Text(
        "Process VRAM usage: %g/%g MiB (%i%%)", 
        static_cast<float>(video_memory_info.CurrentUsage) / (1000000.0f),
        static_cast<float>(Get::Device()->GetAdapter().video_memory) / 1000000.0f,
        static_cast<int>( ( static_cast<float>(video_memory_info.CurrentUsage) / static_cast<float>(Get::Device()->GetAdapter().video_memory) ) * 100.0f)
    );

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    PROCESS_MEMORY_COUNTERS counters;
    counters.cb = sizeof(counters);

    GetProcessMemoryInfo(GetCurrentProcess(), &counters, counters.cb);

    ImGui::Text(
        "Process RAM usage: %g/%g MiB (%i%%)", 
        static_cast<float>(counters.WorkingSetSize) / 1000000.0f,
        static_cast<float>(statex.ullTotalPhys) / 1000000.0f,
        static_cast<int>(((float)counters.WorkingSetSize / (float)statex.ullTotalPhys) * 100.0f)
    );
    ImGui::Text(
        "Global RAM usage: %g/%g MiB (%i%%)", 
        static_cast<float>(statex.ullTotalPhys - statex.ullAvailPhys) / 1000000.0f,
        static_cast<float>(statex.ullTotalPhys) / 1000000.0f, 
        static_cast<int>( ( (float)(statex.ullTotalPhys - statex.ullAvailPhys) / (float)statex.ullTotalPhys ) * 100.0f )
    );

    ImGui::End();

    KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

    float speed = 5.0f;

    if (keyboard.GetKeyDown(KeyCode_LEFT_SHIFT))
        speed *= 10.0f;

    if (keyboard.GetKeyDown(KeyCode_W))
        camera->Translate(DirectX::XMFLOAT3(0.0f, 0.0f, speed * delta_time));

    if (keyboard.GetKeyDown(KeyCode_S))
        camera->Translate(DirectX::XMFLOAT3(0.0f, 0.0f, -speed * delta_time));

    if (keyboard.GetKeyDown(KeyCode_A))
        camera->Translate(DirectX::XMFLOAT3(-speed * delta_time, 0.0f, 0.0f));

    if (keyboard.GetKeyDown(KeyCode_D))
        camera->Translate(DirectX::XMFLOAT3(speed * delta_time, 0.0f, 0.0f));

    MouseState& mouse = Get::MainWindow()->GetMouseState();

    static DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

    if (mouse.GetButtonDown(MouseButton_LEFT))
        camera->Rotate(DirectX::XMFLOAT3(mouse.GetMousePositionDelta().y * 0.005f, mouse.GetMousePositionDelta().x * 0.005f, 0.0f));
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