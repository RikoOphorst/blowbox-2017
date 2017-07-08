#include "blowbox_core.h"

#include "util/assert.h"
#include "core/blowbox_config.h"
#include "core/get.h"
#include "win32/glfw_manager.h"
#include "win32/window.h"
#include "renderer/renderer.h"
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
        config_(config)
    {
        BLOWBOX_ASSERT(config_ != nullptr);

        glfw_manager_ = new GLFWManager();
        renderer_ = new Renderer();
        command_manager_ = new CommandManager();
        command_context_manager_ = new CommandContextManager();

        getter_ = new Get();
        getter_->SetBlowboxCore(this);
        getter_->SetGLFWManager(glfw_manager_);
        getter_->SetRenderer(renderer_);
        getter_->SetCommandManager(command_manager_);
        getter_->SetCommandContextManager(command_context_manager_);
    }

    //------------------------------------------------------------------------------------------------------
    BlowboxCore::~BlowboxCore()
    {
        delete command_context_manager_;
        delete command_manager_;
        delete renderer_;
        delete main_window_;
        delete glfw_manager_;
        delete getter_;
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::Run()
    {
        glfw_manager_->Init();

        main_window_ = new Window(config_->window_resolution, config_->window_title);
        getter_->SetMainWindow(main_window_);
        
        renderer_->Startup();
        getter_->SetDevice(renderer_->GetDevice());

        command_manager_->Startup();
        command_context_manager_->Startup();

        if (user_procedure_run_)
        {
            user_procedure_run_();
        }

        while (IsBlowboxAlive())
        {
            glfw_manager_->Update();

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

            if (user_procedure_post_render_)
            {
                user_procedure_post_render_();
            }
        }

        if (user_procedure_shutdown_)
        {
            user_procedure_shutdown_();
        }

        glfw_manager_->Shutdown();
    }

    //------------------------------------------------------------------------------------------------------
    void BlowboxCore::Shutdown()
    {
        alive_ = false;
    }

    //------------------------------------------------------------------------------------------------------
    bool BlowboxCore::IsBlowboxAlive()
    {
        return alive_;
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