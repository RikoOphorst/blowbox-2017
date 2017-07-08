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
    Renderer* Get::Renderer()
    {
        return Get::instance_->renderer_;
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
    void Get::SetRenderer(blowbox::Renderer* renderer)
    {
        renderer_ = renderer;
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
}