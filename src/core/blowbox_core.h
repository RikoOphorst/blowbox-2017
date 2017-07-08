#pragma once

#include "core/eastl_required.h"
#include <EASTL/functional.h>

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
        */
        bool IsBlowboxAlive();

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
        bool alive_; //!< Tracks whether the engine should still be alive

        eastl::function<void(void)> user_procedure_run_; //!< The procedure that is defined by the user for the Run step
        eastl::function<void(void)> user_procedure_update_; //!< The procedure that is defined by the user for the Update step
        eastl::function<void(void)> user_procedure_post_update_; //!< The procedure that is defined by the user for the PostUpdate step
        eastl::function<void(void)> user_procedure_render_; //!< The procedure that is defined by the user for the Render step
        eastl::function<void(void)> user_procedure_post_render_; //!< The procedure that is defined by the user for the PostRender step
        eastl::function<void(void)> user_procedure_shutdown_; //!< The procedure that is defined by the user for the Shutdown step

        Get* getter_; //!< The Get instance that is used in the entire engine
        GLFWManager* glfw_manager_; //!< The GLFWManager instance is stored here
        Window* main_window_; //!< The main Window instance
    };
}