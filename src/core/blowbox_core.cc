#include "blowbox_core.h"

#include "util/assert.h"
#include "util/delete.h"
#include "core/blowbox_config.h"
#include "core/get.h"

#include "win32/glfw_manager.h"
#include "win32/window.h"

#include "renderer/device.h"
#include "renderer/swap_chain.h"
#include "renderer/forward_renderer.h"
#include "renderer/deferred_renderer.h"
#include "renderer/descriptor_heap.h"
#include "renderer/commands/command_context_manager.h"
#include "renderer/commands/command_manager.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    BlowboxCore::BlowboxCore()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    BlowboxCore::BlowboxCore(BlowboxConfig* config) :
        alive_(true),
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
        render_deferred_renderer_(nullptr)
    {
        BLOWBOX_ASSERT(config_ != nullptr);

        // Create win32 stuff
        win32_glfw_manager_ = new GLFWManager();
        win32_main_window_ = new Window();

        // Create render stuff
        render_device_ = new Device();
        render_swap_chain_ = new SwapChain();
        render_command_manager_ = new CommandManager();
        render_command_context_manager_ = new CommandContextManager();
        render_rtv_heap_ = new DescriptorHeap();
        render_dsv_heap_ = new DescriptorHeap();
        render_cbv_srv_uav_heap_ = new DescriptorHeap();

        render_forward_renderer_ = new ForwardRenderer();
        render_deferred_renderer_ = new DeferredRenderer();

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
        StartupRenderer();

        if (user_procedure_run_)
        {
            user_procedure_run_();
        }

        while (IsBlowboxAlive())
        {
            win32_glfw_manager_->Update();

            if (user_procedure_update_)
            {
                user_procedure_update_();
            }

            if (user_procedure_post_update_)
            {
                user_procedure_post_update_();
            }

            if (user_procedure_render_)
            {
                user_procedure_render_();
            }

            render_forward_renderer_->Render();

            if (user_procedure_post_render_)
            {
                user_procedure_post_render_();
            }
        }

        if (user_procedure_shutdown_)
        {
            user_procedure_shutdown_();
        }

        ShutdownRenderer();
        ShutdownWin32();
        ShutdownGetter();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::Shutdown()
    {
        alive_ = false;
    }

    //------------------------------------------------------------------------------------------------------
    bool BlowboxCore::IsBlowboxAlive()
    {
        if (alive_ == false)
        {
            return false;
        }
        
        if (win32_main_window_->GetWindowShouldClose())
        {
            return false;
        }

        if (win32_main_window_->GetKeyboardState().GetKeyPressed(KeyCode_ESCAPE))
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

        getter_->SetDevice(render_device_);
        getter_->SetCommandManager(render_command_manager_);
        getter_->SetCommandContextManager(render_command_context_manager_);
        getter_->SetRtvHeap(render_rtv_heap_);
        getter_->SetDsvHeap(render_dsv_heap_);
        getter_->SetCbvSrvUavHeap(render_cbv_srv_uav_heap_);
        getter_->SetSwapChain(render_swap_chain_);
        getter_->SetForwardRenderer(render_forward_renderer_);
        getter_->SetDeferredRenderer(render_deferred_renderer_);

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
        auto available_adapters = Device::FindAvailableAdapters(static_cast<FindAdapterFlag>(FindAdapterFlag_NO_INTEL | FindAdapterFlag_NO_SOFTWARE));
        BLOWBOX_ASSERT(available_adapters.size() > 0);
        render_device_->Create(available_adapters[0]);

        // Render command object creation
        render_command_manager_->Startup(render_device_);
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
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::ShutdownGetter()
    {
        BLOWBOX_DELETE(getter_);
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::ShutdownWin32()
    {
        BLOWBOX_DELETE(win32_main_window_);
        BLOWBOX_DELETE(win32_glfw_manager_);
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::ShutdownRenderer()
    {
        render_command_manager_->WaitForIdleGPU();

        BLOWBOX_DELETE(render_forward_renderer_);
        BLOWBOX_DELETE(render_deferred_renderer_);
        BLOWBOX_DELETE(render_swap_chain_);
        BLOWBOX_DELETE(render_cbv_srv_uav_heap_);
        BLOWBOX_DELETE(render_dsv_heap_);
        BLOWBOX_DELETE(render_rtv_heap_);
        BLOWBOX_DELETE(render_command_context_manager_);
        BLOWBOX_DELETE(render_command_manager_);
        BLOWBOX_DELETE(render_device_);
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetRunProcedure(const eastl::function<void(void)>& run_procedure)
    {
        user_procedure_run_ = run_procedure;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetUpdateProcedure(const eastl::function<void(void)>& update_procedure)
    {
        user_procedure_update_ = update_procedure;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetPostUpdateProcedure(const eastl::function<void(void)>& post_update_procedure)
    {
        user_procedure_post_update_ = post_update_procedure;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetRenderProcedure(const eastl::function<void(void)>& render_procedure)
    {
        user_procedure_render_ = render_procedure;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetPostRenderProcedure(const eastl::function<void(void)>& post_render_procedure)
    {
        user_procedure_post_render_ = post_render_procedure;
    }
    
    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::SetShutdownProcedure(const eastl::function<void(void)>& shutdown_procedure)
    {
        user_procedure_shutdown_ = shutdown_procedure;
    }
}