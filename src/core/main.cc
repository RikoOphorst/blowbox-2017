#include <iostream>

#include "util/assert.h"
#include "core/blowbox_core.h"
#include "core/blowbox_config.h"
#include "core/get.h"
#include "win32/window.h"
#include "content/image.h"

#include "core/eastl.h"
#include <EASTL/string.h>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>

using namespace blowbox;

BlowboxCore* blowbox_instance;
Window* main_window;

bool clicked = false;

void Run()
{
    main_window = Get::MainWindow();
    main_window->SetWindowIcon(new Image("icon.png"));
}

void Update()
{
    if (main_window->GetMouseState().GetButtonPressed(MouseButton_4))
    {
        clicked = true;
    }

    if (clicked && main_window->GetMouseState().GetScrollDelta().y > 0.1f)
    {
        blowbox_instance->Shutdown();
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