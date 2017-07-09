#include "get.h"

#include "util/assert.h"

namespace blowbox
{
    Get* Get::instance_ = nullptr;

    //------------------------------------------------------------------------------------------------------
    Get::Get() :
        blowbox_core_(nullptr),
        glfw_manager_(nullptr),
        main_window_(nullptr)
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
    BlowboxCore* Get::BlowboxCore()
    {
        return Get::instance_->blowbox_core_;
    }

    //------------------------------------------------------------------------------------------------------
    GLFWManager* Get::GLFWManager()
    {
        return Get::instance_->glfw_manager_;
    }

    //------------------------------------------------------------------------------------------------------
    Window* Get::MainWindow()
    {
        return Get::instance_->main_window_;
    }

    //------------------------------------------------------------------------------------------------------
    ForwardRenderer* Get::ForwardRenderer()
    {
        return Get::instance_->forward_renderer_;
    }

    //------------------------------------------------------------------------------------------------------
    DeferredRenderer* Get::DeferredRenderer()
    {
        return Get::instance_->deferred_renderer_;
    }

    //------------------------------------------------------------------------------------------------------
    CommandContextManager* Get::CommandContextManager()
    {
        return Get::instance_->command_context_manager_;
    }

    //------------------------------------------------------------------------------------------------------
    CommandManager* Get::CommandManager()
    {
        return Get::instance_->command_manager_;
    }

    //------------------------------------------------------------------------------------------------------
    Device* Get::Device()
    {
        return Get::instance_->device_;
    }

    //------------------------------------------------------------------------------------------------------
    DescriptorHeap* Get::RtvHeap()
    {
        return Get::instance_->rtv_descriptor_heap_;
    }

    //------------------------------------------------------------------------------------------------------
    DescriptorHeap* Get::DsvHeap()
    {
        return Get::instance_->dsv_descriptor_heap_;
    }

    //------------------------------------------------------------------------------------------------------
    DescriptorHeap* Get::CbvSrvUavHeap()
    {
        return Get::instance_->cbv_srv_uav_descriptor_heap_;
    }

    //------------------------------------------------------------------------------------------------------
    SwapChain* Get::SwapChain()
    {
        return Get::instance_->swap_chain_;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetBlowboxCore(blowbox::BlowboxCore* blowbox_core)
    {
        blowbox_core_ = blowbox_core;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetGLFWManager(blowbox::GLFWManager* glfw_manager)
    {
        glfw_manager_ = glfw_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetMainWindow(blowbox::Window* main_window)
    {
        main_window_ = main_window;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetForwardRenderer(blowbox::ForwardRenderer* renderer)
    {
        forward_renderer_ = renderer;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetDeferredRenderer(blowbox::DeferredRenderer* deferred_renderer)
    {
        deferred_renderer_ = deferred_renderer;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetCommandContextManager(blowbox::CommandContextManager* command_context_manager)
    {
        command_context_manager_ = command_context_manager;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetCommandManager(blowbox::CommandManager* command_manager)
    {
        command_manager_ = command_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::SetDevice(blowbox::Device* device)
    {
        device_ = device;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetRtvHeap(blowbox::DescriptorHeap* heap)
    {
        rtv_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetDsvHeap(blowbox::DescriptorHeap* heap)
    {
        dsv_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetCbvSrvUavHeap(blowbox::DescriptorHeap* heap)
    {
        cbv_srv_uav_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetSwapChain(blowbox::SwapChain * swap_chain)
    {
        swap_chain_ = swap_chain;
    }
}