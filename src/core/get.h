#pragma once

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
    class GLFWManager;
    class BlowboxCore;
    class Window;

    /**
    * @class blowbox::Get
    * @author Riko Ophorst
    * @brief This class is meant to provide simple access to all managers in engine
    */
    class Get
    {
        friend class BlowboxCore;
    protected:
        /**
        * @brief Constructs the Get class. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        */
        Get();

        /**
        * @brief Destructs the Get class. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        */
        ~Get();

    public:
        /**
        * @brief Returns the BlowboxCore instance
        * @author Riko Ophorst
        */
        static BlowboxCore* BlowboxCore();

        /**
        * @brief Returns the GLFW manager instance
        * @author Riko Ophorst
        */
        static GLFWManager* GLFWManager();

        /**
        * @brief Returns the main Window instance
        * @author Riko Ophorst
        */
        static Window* MainWindow();
        
    protected:
        /**
        * @brief Sets the BlowboxCore instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] blowbox_core   The instance of BlowboxCore
        */
        void SetBlowboxCore(blowbox::BlowboxCore* blowbox_core);

        /**
        * @brief Sets the GLFWManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] glfw_manager   The instance of GLFWManager
        */
        void SetGLFWManager(blowbox::GLFWManager* glfw_manager);

        /**
        * @brief Sets the main Window instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] main_window   The instance of Window
        */
        void SetMainWindow(blowbox::Window* main_window);

        static Get* instance_; //!< The instance of the Get class

    private:
        blowbox::BlowboxCore* blowbox_core_; //!< The BlowboxCore instance
        blowbox::GLFWManager* glfw_manager_; //!< The GLFWManager instance
        blowbox::Window* main_window_; //!< The main Window instance
    };
}