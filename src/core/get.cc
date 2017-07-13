#include "get.h"

#include "util/assert.h"

namespace blowbox
{
    Get* Get::instance_ = nullptr;

    //------------------------------------------------------------------------------------------------------
    Get::Get() :
        finalized_(false),
        blowbox_core_(nullptr)
    {
        BLOWBOX_ASSERT(instance_ == nullptr);
        instance_ = this;
    }

    //------------------------------------------------------------------------------------------------------
    Get::~Get()
    {
        instance_ = nullptr;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Finalize()
    {
        BLOWBOX_ASSERT(blowbox_core_                            != nullptr);
        BLOWBOX_ASSERT(glfw_manager_.use_count()                > 0);
        BLOWBOX_ASSERT(main_window_.use_count()                 > 0);
        BLOWBOX_ASSERT(forward_renderer_.use_count()            > 0);
        BLOWBOX_ASSERT(deferred_renderer_.use_count()           > 0);
        BLOWBOX_ASSERT(command_context_manager_.use_count()     > 0);
        BLOWBOX_ASSERT(command_manager_.use_count()             > 0);
        BLOWBOX_ASSERT(device_.use_count()                      > 0);
        BLOWBOX_ASSERT(rtv_descriptor_heap_.use_count()         > 0);
        BLOWBOX_ASSERT(dsv_descriptor_heap_.use_count()         > 0);
        BLOWBOX_ASSERT(cbv_srv_uav_descriptor_heap_.use_count() > 0);
        BLOWBOX_ASSERT(swap_chain_.use_count()                  > 0);
        BLOWBOX_ASSERT(imgui_manager_.use_count()               > 0);
        BLOWBOX_ASSERT(scene_manager_.use_count()               > 0);

        finalized_ = true;
    }

    //------------------------------------------------------------------------------------------------------
	BlowboxCore* Get::BlowboxCore()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->blowbox_core_;
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<GLFWManager> Get::GLFWManager()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->glfw_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr< Window> Get::MainWindow()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->main_window_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<ForwardRenderer> Get::ForwardRenderer()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->forward_renderer_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<DeferredRenderer> Get::DeferredRenderer()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->deferred_renderer_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<CommandContextManager> Get::CommandContextManager()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->command_context_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<CommandManager> Get::CommandManager()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->command_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<Device> Get::Device()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->device_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<DescriptorHeap> Get::RtvHeap()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->rtv_descriptor_heap_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<DescriptorHeap> Get::DsvHeap()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->dsv_descriptor_heap_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<DescriptorHeap> Get::CbvSrvUavHeap()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->cbv_srv_uav_descriptor_heap_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<SwapChain> Get::SwapChain()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->swap_chain_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<ImGuiManager> Get::ImGuiManager()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->imgui_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	eastl::shared_ptr<SceneManager> Get::SceneManager()
    {
        return Get::instance_->scene_manager_.lock();
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetBlowboxCore(blowbox::BlowboxCore* blowbox_core)
    {
		blowbox_core_ = blowbox_core;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetGLFWManager(eastl::shared_ptr<blowbox::GLFWManager> glfw_manager)
    {
        glfw_manager_ = glfw_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetMainWindow(eastl::shared_ptr<blowbox::Window> main_window)
    {
        main_window_ = main_window;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetForwardRenderer(eastl::shared_ptr<blowbox::ForwardRenderer> renderer)
    {
        forward_renderer_ = renderer;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetDeferredRenderer(eastl::shared_ptr<blowbox::DeferredRenderer> deferred_renderer)
    {
        deferred_renderer_ = deferred_renderer;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetCommandContextManager(eastl::shared_ptr<blowbox::CommandContextManager> command_context_manager)
    {
        command_context_manager_ = command_context_manager;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetCommandManager(eastl::shared_ptr<blowbox::CommandManager> command_manager)
    {
        command_manager_ = command_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetDevice(eastl::shared_ptr<blowbox::Device> device)
    {
        device_ = device;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetRtvHeap(eastl::shared_ptr<blowbox::DescriptorHeap> heap)
    {
        rtv_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetDsvHeap(eastl::shared_ptr<blowbox::DescriptorHeap> heap)
    {
        dsv_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetCbvSrvUavHeap(eastl::shared_ptr<blowbox::DescriptorHeap> heap)
    {
        cbv_srv_uav_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetSwapChain(eastl::shared_ptr<blowbox::SwapChain> swap_chain)
    {
        swap_chain_ = swap_chain;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetImGuiManager(eastl::shared_ptr<blowbox::ImGuiManager> imgui_manager)
    {
        imgui_manager_ = imgui_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetSceneManager(eastl::shared_ptr<blowbox::SceneManager> scene_manager)
    {
        scene_manager_ = scene_manager;
    }
}