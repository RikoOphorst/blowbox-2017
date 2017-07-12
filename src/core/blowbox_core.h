#pragma once

#include "core/eastl.h"
#include <EASTL/functional.h>

#define BLOWBOX_SWAP_CHAIN_BUFFER_COUNT 2
#define BLOWBOX_DESCRIPTOR_HEAP_MAX_RTV_COUNT 1024U
#define BLOWBOX_DESCRIPTOR_HEAP_MAX_DSV_COUNT 1024U
#define BLOWBOX_DESCRIPTOR_HEAP_MAX_CBV_SRV_UAV_COUNT 16384U

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
    class Get;
    struct BlowboxConfig;
    class GLFWManager;
    class Window;
    class Device;
    class SwapChain;
    class CommandManager;
    class CommandContextManager;
    class DescriptorHeap;
    class ForwardRenderer;
    class DeferredRenderer;
    class ImGuiManager;

    /**
    * @class blowbox::BlowboxCore
    * @author Riko Ophorst
    * @brief This is the core of blowbox. It sets up all subsystems and manages them accordingly.
    */
    class BlowboxCore
    {
    public:
        /**
        * @brief This is a dummy constructor and doesn't do anything except that it enables this class to have a default constructor
        * @author Riko Ophorst
        */
        BlowboxCore();

        /**
        * @brief Constructs the BlowboxCore.
        * @author Riko Ophorst
        * @param[in] The configuration you wish to use for the BlowboxCore instance
        */
        BlowboxCore(BlowboxConfig* config);

        /**
        * @brief Delete the copy constructor
        * @author Riko Ophorst
        */
        BlowboxCore(const BlowboxCore& that) = delete;

        /**
        * @brief Destroys the BlowboxCore.
        * @author Riko Ophorst
        */
        ~BlowboxCore();

        /**
        * @brief Kicks off the gameloop. Keep in mind that this function is blocking until the game is exited.
        */
        void Run();

        /**
        * @brief Triggers a shutdown upon enter of the next frame
        */
        void Shutdown();

    protected:
        /**
        * @brief Figures out whether the BlowboxCore should shutdown based on the state of the engine (window, input, etc)
        * @author Riko Ophorst
        */
        bool IsBlowboxAlive();

        /**
        * @brief Starts up the Getter system.
        * @author Riko Ophorst
        */
        void StartupGetter();

        /**
        * @brief Starts up the Win32 subsystems
        * @author Riko Ophorst
        */
        void StartupWin32();

        /**
        * @brief Starts up the Renderer subsystems
        * @author Riko Ophorst
        */
        void StartupRenderer();

        /**
        * @brief Shuts down the Getter system
        * @author Riko Ophorst
        */
        void ShutdownGetter();

        /**
        * @brief Shuts down the Win32 subsystems
        * @author Riko Ophorst
        */
        void ShutdownWin32();

        /**
        * @brief Shuts down the Renderer subsystems
        * @author Riko Ophorst
        */
        void ShutdownRenderer();

        void Update();
        void Render();

    public:
        /**
        * @brief Set a function that gets called whenever the Run step happens in the game loop
        * @param[in] run_procedure	The function that should be called when the Run step happens in the game loop
        */
        void SetRunProcedure(const eastl::function<void(void)>& run_procedure);

        /**
        * @brief Set a function that gets called whenever the Update step happens in the game loop
        * @param[in] update_procedure	The function that should be called each frame when the Update step happens in the game loop
        */
        void SetUpdateProcedure(const eastl::function<void(void)>& update_procedure);

        /**
        * @brief Set a function that gets called whenever the PostUpdate step happens in the game loop
        * @param[in] post_update_procedure	The function that should be called each frame when the PostUpdate step happens in the game loop
        */
        void SetPostUpdateProcedure(const eastl::function<void(void)>& post_update_procedure);

        /**
        * @brief Set a function that gets called whenever the Render step happens in the game loop
        * @param[in] render_procedure	The function that should be called each frame when the Render step happens in the game loop
        */
        void SetRenderProcedure(const eastl::function<void(void)>& render_procedure);

        /**
        * @brief Set a function that gets called whenever the PostRender step happens in the game loop
        * @param[in] post_render_procedure	The function that should be called each frame when the PostRender step happens in the game loop
        */
        void SetPostRenderProcedure(const eastl::function<void(void)>& post_render_procedure);

        /**
        * @brief Set a function that gets called whenever the Shutdown step happens in the game loop
        * @param[in] shutdown_procedure	The function that should be called when the Shutdown step happens in the game loop
        */
        void SetShutdownProcedure(const eastl::function<void(void)>& shutdown_procedure);

    private:
        BlowboxConfig* config_; //!< The configuration of blowbox
        bool shutdown_requested_by_external_; //!< Tracks whether the engine should still be alive

        eastl::function<void(void)> user_procedure_run_; //!< The procedure that is defined by the user for the Run step
        eastl::function<void(void)> user_procedure_update_; //!< The procedure that is defined by the user for the Update step
        eastl::function<void(void)> user_procedure_post_update_; //!< The procedure that is defined by the user for the PostUpdate step
        eastl::function<void(void)> user_procedure_render_; //!< The procedure that is defined by the user for the Render step
        eastl::function<void(void)> user_procedure_post_render_; //!< The procedure that is defined by the user for the PostRender step
        eastl::function<void(void)> user_procedure_shutdown_; //!< The procedure that is defined by the user for the Shutdown step

        Get* getter_; //!< The Get instance that is used in the entire engine
        
        // win32 stuff
        GLFWManager* win32_glfw_manager_; //!< The GLFWManager instance is stored here
        Window* win32_main_window_; //!< The main Window instance

        // render stuff
        Device* render_device_; //!< The Device used by the renderers
        SwapChain* render_swap_chain_; //!< The SwapChain used by the renderers
        CommandManager* render_command_manager_; //!< The CommandManager instance
        CommandContextManager* render_command_context_manager_; //!< The CommandContextManager instance
        DescriptorHeap* render_rtv_heap_; //!< DescriptorHeap for render target views
        DescriptorHeap* render_dsv_heap_; //!< DescriptorHeap for depth stencil views
        DescriptorHeap* render_cbv_srv_uav_heap_; //!< DescriptorHeap for cbv/srv/uavs

        ForwardRenderer* render_forward_renderer_; //!< The ForwardRenderer instance
        DeferredRenderer* render_deferred_renderer_; //!< The DeferredRenderer instance

        ImGuiManager* render_imgui_manager_; //!< The ImGuiManager instance

        bool show_test_window_;

        // content stuff
        // nothing yet
    };
}