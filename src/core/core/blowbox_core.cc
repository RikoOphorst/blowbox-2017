#include "blowbox_core.h"

#include "util/assert.h"
#include "util/delete.h"

#include "core/get.h"
#include "core/core/blowbox_config.h"
#include "core/scene/scene_manager.h"

#include "core/debug/debug_menu.h"
#include "core/debug/console.h"
#include "core/debug/profiler.h"

#include "win32/glfw_manager.h"
#include "win32/window.h"
#include "win32/time.h"

#include "renderer/device.h"
#include "renderer/swap_chain.h"
#include "renderer/forward_renderer.h"
#include "renderer/deferred_renderer.h"
#include "renderer/descriptor_heap.h"
#include "renderer/commands/command_context_manager.h"
#include "renderer/commands/command_manager.h"
#include "renderer/commands/graphics_context.h"
#include "renderer/imgui/imgui_manager.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    BlowboxCore::BlowboxCore()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    BlowboxCore::BlowboxCore(BlowboxConfig* config) :
        shutdown_requested_(false),
        config_(config),
        getter_(nullptr),
        win32_glfw_manager_(nullptr),
        win32_main_window_(nullptr),
        render_device_(nullptr),
        render_swap_chain_(nullptr),
        render_command_manager_(nullptr),
        render_command_context_manager_(nullptr),
        render_rtv_heap_(nullptr),
        render_dsv_heap_(nullptr),
        render_cbv_srv_uav_heap_(nullptr),
        render_forward_renderer_(nullptr),
        render_deferred_renderer_(nullptr),
        render_imgui_manager_(nullptr)
    {
        BLOWBOX_ASSERT(config_ != nullptr);

        // Create win32 stuff
        win32_glfw_manager_ = eastl::make_shared<GLFWManager>();
        win32_main_window_ = eastl::make_shared<Window>();
        win32_time_ = eastl::make_shared<Time>();

        // Create render stuff
		render_device_ = eastl::make_shared<Device>();
        render_swap_chain_ = eastl::make_shared<SwapChain>();
        render_command_manager_ = eastl::make_shared<CommandManager>();
        render_command_context_manager_ = eastl::make_shared<CommandContextManager>();
        render_rtv_heap_ = eastl::make_shared<DescriptorHeap>();
        render_dsv_heap_ = eastl::make_shared<DescriptorHeap>();
        render_cbv_srv_uav_heap_ = eastl::make_shared<DescriptorHeap>();

        render_forward_renderer_ = eastl::make_shared<ForwardRenderer>();
        render_deferred_renderer_ = eastl::make_shared<DeferredRenderer>();

        render_imgui_manager_ = eastl::make_shared<ImGuiManager>();

        // Create Scene stuff
		scene_manager_ = eastl::make_shared<SceneManager>();

        // Create debug stuff
        debug_menu_ = eastl::make_shared<DebugMenu>();
        console_ = eastl::make_shared<Console>();
        profiler_ = eastl::make_shared<Profiler>();

        // Create Getter
		getter_ = new Get();
    }

    //------------------------------------------------------------------------------------------------------
    BlowboxCore::~BlowboxCore()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::Run()
    {
        StartupGetter();
        StartupWin32();
        StartupDebug();
        StartupRenderer();
        StartupScene();

        if (user_procedure_run_)
        {
            user_procedure_run_();
        }

        while (IsBlowboxAlive())
        {
            win32_glfw_manager_->Update();
            win32_time_->NewFrame();
            profiler_->NewFrame();
            render_imgui_manager_->NewFrame();

            Update();

            Render();
        }

        if (user_procedure_shutdown_)
        {
            user_procedure_shutdown_();
        }

        ShutdownScene();
        ShutdownRenderer();
        ShutdownDebug();
        ShutdownWin32();
        ShutdownGetter();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::Shutdown()
    {
        shutdown_requested_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    bool BlowboxCore::IsBlowboxAlive()
    {
        if (shutdown_requested_ == true)
        {
            return false;
        }
        
        if (win32_main_window_->GetWindowShouldClose())
        {
            return false;
        }

        return true;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::StartupGetter()
    {
        getter_->SetBlowboxCore(this);

        getter_->SetGLFWManager(win32_glfw_manager_);
        getter_->SetMainWindow(win32_main_window_);
        getter_->SetTime(win32_time_);

        getter_->SetDevice(render_device_);
        getter_->SetCommandManager(render_command_manager_);
        getter_->SetCommandContextManager(render_command_context_manager_);
        getter_->SetRtvHeap(render_rtv_heap_);
        getter_->SetDsvHeap(render_dsv_heap_);
        getter_->SetCbvSrvUavHeap(render_cbv_srv_uav_heap_);
        getter_->SetSwapChain(render_swap_chain_);
        getter_->SetForwardRenderer(render_forward_renderer_);
        getter_->SetDeferredRenderer(render_deferred_renderer_);
        getter_->SetImGuiManager(render_imgui_manager_);

		getter_->SetSceneManager(scene_manager_);

        getter_->SetDebugMenu(debug_menu_);
        getter_->SetConsole(console_);
        getter_->SetProfiler(profiler_);

        getter_->Finalize();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::StartupWin32()
    {
        win32_glfw_manager_->Init();
        win32_main_window_->Create(config_->window_resolution, config_->window_title);
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::StartupRenderer()
    {
        // Render device creation
        auto available_adapters = Adapter::FindAvailableAdapters(static_cast<FindAdapterFlag>(FindAdapterFlag_NO_INTEL | FindAdapterFlag_NO_SOFTWARE));
        BLOWBOX_ASSERT(available_adapters.size() > 0);
        render_device_->Create(available_adapters[0]);

        // Render command object creation
        render_command_manager_->Startup();
        render_command_context_manager_->Startup();

        // Render descriptor heap creation
        render_rtv_heap_->Create(           L"DescriptorHeapRTV",           D3D12_DESCRIPTOR_HEAP_TYPE_RTV,         D3D12_DESCRIPTOR_HEAP_FLAG_NONE,            BLOWBOX_DESCRIPTOR_HEAP_MAX_RTV_COUNT);
		render_dsv_heap_->Create(           L"DescriptorHeapDSV",           D3D12_DESCRIPTOR_HEAP_TYPE_DSV,         D3D12_DESCRIPTOR_HEAP_FLAG_NONE,            BLOWBOX_DESCRIPTOR_HEAP_MAX_DSV_COUNT);
		render_cbv_srv_uav_heap_->Create(   L"DescriptorHeapCBV_SRV_UAV",   D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,  BLOWBOX_DESCRIPTOR_HEAP_MAX_CBV_SRV_UAV_COUNT);

        // Render swap chain creation
        render_swap_chain_->Create(
            win32_main_window_->GetWindowHandle(),
            win32_main_window_->GetWindowResolution().width,
            win32_main_window_->GetWindowResolution().height,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            BLOWBOX_SWAP_CHAIN_BUFFER_COUNT,
            DXGI_SWAP_EFFECT_FLIP_DISCARD,
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        );

        render_forward_renderer_->Startup();

        render_imgui_manager_->Init();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::StartupScene()
    {
        scene_manager_->Startup();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::StartupDebug()
    {
        debug_menu_->AddDebugWindow(1, console_);
        debug_menu_->AddDebugWindow(2, profiler_);
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::ShutdownGetter()
    {
		BLOWBOX_DELETE(getter_);
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::ShutdownWin32()
    {
        BLOWBOX_ASSERT(win32_main_window_.use_count() == 1);
        BLOWBOX_ASSERT(win32_glfw_manager_.use_count() == 1);
        BLOWBOX_ASSERT(win32_time_.use_count() == 1);

        win32_time_.reset();
        win32_main_window_.reset();
        win32_glfw_manager_.reset();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::ShutdownRenderer()
    {
        render_command_manager_->WaitForIdleGPU();
        render_imgui_manager_->Shutdown();

        BLOWBOX_ASSERT(render_imgui_manager_.use_count() == 1);
        BLOWBOX_ASSERT(render_forward_renderer_.use_count() == 1);
        BLOWBOX_ASSERT(render_deferred_renderer_.use_count() == 1);
        BLOWBOX_ASSERT(render_swap_chain_.use_count() == 1);
        BLOWBOX_ASSERT(render_cbv_srv_uav_heap_.use_count() == 1);
        BLOWBOX_ASSERT(render_dsv_heap_.use_count() == 1);
        BLOWBOX_ASSERT(render_rtv_heap_.use_count() == 1);
        BLOWBOX_ASSERT(render_command_context_manager_.use_count() == 1);
        BLOWBOX_ASSERT(render_command_manager_.use_count() == 1);
        BLOWBOX_ASSERT(render_device_.use_count() == 1);

        render_imgui_manager_.reset();
        render_forward_renderer_.reset();
        render_deferred_renderer_.reset();
        render_swap_chain_.reset();
        render_cbv_srv_uav_heap_.reset();
        render_dsv_heap_.reset();
        render_rtv_heap_.reset();
        render_command_context_manager_.reset();
        render_command_manager_.reset();
        render_device_.reset();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::ShutdownScene()
    {
        scene_manager_->Shutdown();

        BLOWBOX_ASSERT(scene_manager_.use_count() == 1);
        scene_manager_.reset();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::ShutdownDebug()
    {
        BLOWBOX_ASSERT(debug_menu_.use_count() == 1);
        BLOWBOX_ASSERT(console_.use_count() == 1);

        console_.reset();
        debug_menu_.reset();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::Update()
    {
        if (user_procedure_update_)
        {
            Profiler::ProfilerBlock block("UserProcedure: Update", ProfilerBlockType_CORE);
            user_procedure_update_();
        }

		scene_manager_->Update();

        if (ImGui::BeginPopupModal("Exit?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to exit?");
            ImGui::Separator();

            if (ImGui::Button("OK", ImVec2(120, 0)) || Get::MainWindow()->GetKeyboardState().GetKeyPressed(KeyCode_ENTER))
            {
                ImGui::CloseCurrentPopup();
                Shutdown();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(120, 0)) || Get::MainWindow()->GetKeyboardState().GetKeyPressed(KeyCode_ESCAPE))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        else
        {
            if (Get::MainWindow()->GetKeyboardState().GetKeyPressed(KeyCode_ESCAPE))
            {
                // Make a temp window that catches focus. This is because an imgui window needs to have focus before it can open a modal popup like our exit prompt.
                ImGui::Begin("Window that fixes a weird ass bug in imgui.");
                ImGui::SetWindowFocus();
                ImGui::End();
                ImGui::OpenPopup("Exit?");
            }
        }

        if (user_procedure_post_update_)
        {
            Profiler::ProfilerBlock block("UserProcedure: PostUpdate", ProfilerBlockType_CORE);
            user_procedure_post_update_();
        }

		scene_manager_->PostUpdate();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::Render()
    {
        // Pre render user procedure
        if (user_procedure_render_)
        {
            Profiler::ProfilerBlock block("UserProcedure: Render", ProfilerBlockType_CORE);
            user_procedure_render_();
        }

        // Frame start
        {
            Profiler::ProfilerBlock block("FrameStart", ProfilerBlockType_RENDERER);
            GraphicsContext& context_frame_start = GraphicsContext::Begin(L"CommandListFrameStart");

            ID3D12DescriptorHeap* heaps[1] = { render_cbv_srv_uav_heap_->Get() };
            D3D12_DESCRIPTOR_HEAP_TYPE heap_types[1] = { D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV };
            context_frame_start.SetDescriptorHeaps(1, heap_types, heaps);
            context_frame_start.TransitionResource(render_swap_chain_->GetBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET);
            context_frame_start.Finish();
        }

        // Do actual rendering
        {
            render_forward_renderer_->Render();

            debug_menu_->RenderMenu();
            debug_menu_->RenderWindows();

            render_imgui_manager_->Render();
        }

        // Frame end
        {
            Profiler::ProfilerBlock block("FrameEnd", ProfilerBlockType_RENDERER);
            GraphicsContext& context_frame_end = GraphicsContext::Begin(L"CommandListFrameEnd");

            context_frame_end.TransitionResource(render_swap_chain_->GetBackBuffer(), D3D12_RESOURCE_STATE_PRESENT);
            context_frame_end.Finish(true);

            render_swap_chain_->Present(false);
        }

        // Post render user procedure
        if (user_procedure_post_render_)
        {
            Profiler::ProfilerBlock block("UserProcedure: PostRender", ProfilerBlockType_CORE);
            user_procedure_post_render_();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetRunProcedure(const Function<void>& run_procedure)
    {
        user_procedure_run_ = run_procedure;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetUpdateProcedure(const Function<void>& update_procedure)
    {
        user_procedure_update_ = update_procedure;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetPostUpdateProcedure(const Function<void>& post_update_procedure)
    {
        user_procedure_post_update_ = post_update_procedure;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetRenderProcedure(const Function<void>& render_procedure)
    {
        user_procedure_render_ = render_procedure;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetPostRenderProcedure(const Function<void>& post_render_procedure)
    {
        user_procedure_post_render_ = post_render_procedure;
    }
    
    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetShutdownProcedure(const Function<void>& shutdown_procedure)
    {
        user_procedure_shutdown_ = shutdown_procedure;
    }
}