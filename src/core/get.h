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
    class Renderer;
    class CommandManager;
    class CommandContextManager;
    class Device;

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

        /**
        * @brief Returns the Renderer instance
        * @author Riko Ophorst
        */
        static Renderer* Renderer();

        /**
        * @brief Returns the CommandContextManager instance
        * @author Riko Ophorst
        */
        static CommandContextManager* CommandContextManager();

        /**
        * @brief Returns the CommandManager instance
        * @author Riko Ophorst
        */
        static CommandManager* CommandManager();

        /**
        * @brief Returns the Device instance
        * @author Riko Ophorst
        */
        static Device* Device();
        
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

        /**
        * @brief Sets the Renderer instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] renderer       The instance of Renderer
        */
        void SetRenderer(blowbox::Renderer* renderer);

        /**
        * @brief Sets the CommandContextManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] command_context_manager       The instance of CommandContextManager
        */
        void SetCommandContextManager(blowbox::CommandContextManager* command_context_manager);

        /**
        * @brief Sets the CommandManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] command_manager       The instance of CommandManager
        */
        void SetCommandManager(blowbox::CommandManager* command_manager);

        /**
        * @brief Sets the Device instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] device       The instance of Device
        */
        void SetDevice(blowbox::Device* device);

        static Get* instance_; //!< The instance of the Get class

    private:
        blowbox::BlowboxCore* blowbox_core_; //!< The BlowboxCore instance
        blowbox::GLFWManager* glfw_manager_; //!< The GLFWManager instance
        blowbox::Window* main_window_; //!< The main Window instance
        blowbox::Renderer* renderer_; //!< The Renderer that is used to render stuff to the main Window
        blowbox::CommandContextManager* command_context_manager_; //!< The CommandContextManager instance
        blowbox::CommandManager* command_manager_; //!< The CommandManager instance
        blowbox::Device* device_; //!< The render Device instance
    };
}