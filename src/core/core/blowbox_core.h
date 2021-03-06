#pragma once

#include "util/functional.h"
#include "util/shared_ptr.h"

#define BLOWBOX_SWAP_CHAIN_BUFFER_COUNT 2
#define BLOWBOX_DESCRIPTOR_HEAP_MAX_RTV_COUNT 8192U
#define BLOWBOX_DESCRIPTOR_HEAP_MAX_DSV_COUNT 256U
#define BLOWBOX_DESCRIPTOR_HEAP_MAX_CBV_SRV_UAV_COUNT 16384U

/**
* All symbols that are in Blowbox exist in the blowbox namespace.
* Because the design philosophy behind Blowbox is based on flat
* hierarchies, there are no other namespaces in the entirity of
* Blowbox except for blowbox itself.
*
* @brief The main Blowbox namespace.
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
	class SceneManager;
    class DebugMenu;
    class Console;
    class Time;
    class PerformanceProfiler;
    class MemoryProfiler;
    class ImageManager;
    class FileManager;
    class TextureManager;
    class MaterialManager;

    /**
    * This is the main class that the user has to create upon startup. It sets up everything
    * for you. All necessary subsystems are initialized and setup. You can start executing
    * Blowbox by calling BlowboxCore::Run(). Before doing so, you have the option to set a 
    * few user procedures that happen at different times during the execution of the application.
    * These are provided via functions such as BlowboxCore::SetUpdateProcedure(). BlowboxCore
    * can only be created through the BlowboxCore::BlowboxCore(BlowboxConfig* config) constructor,
    * because a config is mandatory.
    *
    * @brief The core of Blowbox.
    */
    class BlowboxCore
    {
    public:
        /**
        * @brief This is a dummy constructor and doesn't do anything except that it enables this class to have a default constructor
        * @remarks This constructor is not valid and only exists so that the BlowboxCore has a default constructor.
        */
        BlowboxCore();

        /**
        * @brief Constructs the actual BlowboxCore.
        * @param[in] config The configuration you wish to use for the BlowboxCore instance
        * @remarks The subsystems aren't initialized yet, they are merely constructed. They only get
        *          initialized when you call BlowboxCore::Run().
        */
        BlowboxCore(BlowboxConfig* config);

        /** @brief The copy destructor is unavailable. A BlowboxCore isn't copyable. */
        BlowboxCore(const BlowboxCore& that) = delete;

        /** @brief Destroys the BlowboxCore. */
        ~BlowboxCore();

        /**
        * This starts the actual gameloop. All subsystems will be properly initialized and retrievable
        * through the Get class from this point on. This function is blocking until the game is exited.
        * You can manually trigger an exit by calling BlowboxCore::Shutdown() from one of the user procedures.
        * First, all subsystems are initialized. Second, the game loop is entered. Each frame the game
        * gets updated and rendered. After the game loop is done, subsystems automatically get shutdown.
        * Once BlowboxCore::Run() returns, you are free to delete the BlowboxCore instance from the heap.
        *
        * @brief Kicks off the gameloop.
        */
        void Run();

        /**
        * This function requests BlowboxCore to shutdown. BlowboxCore will finish the current frame and
        * then continue to shutdown all the different subsystems.
        *
        * @brief Request BlowboxCore to shutdown.
        * @remarks Should only be called from one of the customizable user procedures.
        */
        void Shutdown();

        /** @brief Figures out whether the BlowboxCore should shutdown based on the state of the engine (window, input, etc) */
        bool IsBlowboxAlive();

    protected:

        /** @brief Starts up the getter system. */
        void StartupGetter();

        /** @brief Starts up the content subsystems. */
        void StartupContent();

        /** @brief Starts up the win32 subsystems. */
        void StartupWin32();

        /** @brief Starts up the renderer subsystems. */
        void StartupRenderer();

        /** @brief Starts up the scene subsystems. */
        void StartupScene();

        /** @brief Starts up the debug subsystems. */
        void StartupDebug();

        /** @brief Shuts down the getter system. */
        void ShutdownGetter();

        /** @brief Shuts down the content subsystems. */
        void ShutdownContent();

        /** @brief Shuts down the win32 subsystems. */
        void ShutdownWin32();

        /** @brief Shuts down the renderer subsystems. */
        void ShutdownRenderer();

        /** @brief Shuts down the scene subsystems. */
        void ShutdownScene();

        /** @brief Shuts down the debug subsystems. */
        void ShutdownDebug();

        /**
        * This function updates all subsystems in the engine in the correct order.
        * User procedures are also called by this function.
        *
        * @brief Performs the update step in the game loop. 
        */
        void Update();

        /**
        * This function triggers the rendering of the game to the main Window instance.
        * User procedures are also called by this function.
        *
        * @brief Performs the render step in the game loop.
        */
        void Render();

    public:
        /**
        * The Run procedure is invoked immediately after all subsystems have been
        * initialized (after calling BlowboxCore::Run()). It allows you to enter
        * some miscellaneous code before the actual game loop starts.
        *
        * @brief Sets the Run user procedure.
        * @param[in] run_procedure	The function that should be called when the Run step happens in the game loop
        */
        void SetRunProcedure(const Function<void>& run_procedure);

        /**
        * The Update procedure is invoked every frame before the entire scene 
        * is updated. It allows you to enter some miscellaneous code before the
        * game gets updated, essentially.
        *
        * @brief Sets the Update user procedure.
        * @param[in] update_procedure	The function that should be called each frame when the Update step happens in the game loop
        */
        void SetUpdateProcedure(const Function<void>& update_procedure);

        /**
        * The PostUpdate procedure is invoked every frame after the entire scene 
        * is updated. It allows you to enter some miscellaneous code after the
        * game gets updated, essentially.
        *
        * @brief Set the PostUpdate user procedure.
        * @param[in] post_update_procedure	The function that should be called each frame when the PostUpdate step happens in the game loop
        */
        void SetPostUpdateProcedure(const Function<void>& post_update_procedure);

        /**
        * The Render procedure is invoked every frame before the entire scene is
        * rendered. It allows you to enter some miscellaneous code before the
        * game gets rendered, essentially.
        *
        * @brief Sets the Render user procedure.
        * @param[in] render_procedure	The function that should be called each frame when the Render step happens in the game loop
        */
        void SetRenderProcedure(const Function<void>& render_procedure);

        /**
        * The PostRender procedure is invoked every frame after the entire scene 
        * is rendered. It allows you to enter some miscellaneous code after the
        * game gets rendered, essentially.
        *
        * @brief Sets the PostRender user procedure.
        * @param[in] post_render_procedure	The function that should be called each frame when the PostRender step happens in the game loop
        */
        void SetPostRenderProcedure(const Function<void>& post_render_procedure);

        /**
        * The Shutdown procedure is invoked right after the final frame has been
        * executed by the game loop, but before all subsystems get shutdown. It
        * allows you to enter some miscellaneous code after the game gets shutdown.
        *
        * @brief Sets the Shutdown user procedure.
        * @param[in] shutdown_procedure	The function that should be called when the Shutdown step happens in the game loop
        */
        void SetShutdownProcedure(const Function<void>& shutdown_procedure);

    private:
        BlowboxConfig* config_;                                             //!< The configuration of blowbox.
        bool shutdown_requested_;                                           //!< Tracks whether the engine should still be alive.

        Function<void> user_procedure_run_;                                 //!< The procedure that is defined by the user for the Run step.
        Function<void> user_procedure_update_;                              //!< The procedure that is defined by the user for the Update step.
        Function<void> user_procedure_post_update_;                         //!< The procedure that is defined by the user for the PostUpdate step.
        Function<void> user_procedure_render_;                              //!< The procedure that is defined by the user for the Render step.
        Function<void> user_procedure_post_render_;                         //!< The procedure that is defined by the user for the PostRender step.
        Function<void> user_procedure_shutdown_;                            //!< The procedure that is defined by the user for the Shutdown step.

		Get* getter_;                                                       //!< The Get instance that is used in the entire engine.
        
        // win32 stuff
		SharedPtr<GLFWManager> win32_glfw_manager_;                         //!< The GLFWManager instance is stored here.
		SharedPtr<Window> win32_main_window_;                               //!< The main Window instance.
        SharedPtr<Time> win32_time_;                                        //!< The Time instance.

        // content stuff
        SharedPtr<ImageManager> content_image_manager_;                     //!< The ImageManager instance.
        SharedPtr<FileManager> content_file_manager_;                       //!< The FileManager isntance.

        // render stuff
        SharedPtr<Device> render_device_;                                   //!< The Device used by the renderers.
		SharedPtr<SwapChain> render_swap_chain_;                            //!< The SwapChain used by the renderers.
        SharedPtr<CommandManager> render_command_manager_;                  //!< The CommandManager instance.
        SharedPtr<CommandContextManager> render_command_context_manager_;   //!< The CommandContextManager instance.
        SharedPtr<DescriptorHeap> render_rtv_heap_;                         //!< DescriptorHeap for render target views.
        SharedPtr<DescriptorHeap> render_dsv_heap_;                         //!< DescriptorHeap for depth stencil views.
        SharedPtr<DescriptorHeap> render_cbv_srv_uav_heap_;                 //!< DescriptorHeap for cbv/srv/uavs.
        SharedPtr<TextureManager> render_texture_manager_;                  //!< The TextureManager instance.
        SharedPtr<MaterialManager> render_material_manager_;                //!< The MaterialManager instance.

		SharedPtr<ForwardRenderer> render_forward_renderer_;                //!< The ForwardRenderer instance.
		SharedPtr<DeferredRenderer> render_deferred_renderer_;              //!< The DeferredRenderer instance.

        SharedPtr<ImGuiManager> render_imgui_manager_;                      //!< The ImGuiManager instance.

        // scene stuff
		SharedPtr<SceneManager> scene_manager_;                             //!< The SceneManager instance.

        // debug stuff
        SharedPtr<DebugMenu> debug_menu_;                                   //!< The DebugMenu instance.
        SharedPtr<Console> console_;                                        //!< The Console instance.
        SharedPtr<PerformanceProfiler> performance_profiler_;               //!< The PerformanceProfiler instance.
        SharedPtr<MemoryProfiler> memory_profiler_;                         //!< The MemoryProfiler instance.

    public:
        static bool alive;                                                  //!< Static variable to determine whether the BlowboxCore is alive.
    };
}