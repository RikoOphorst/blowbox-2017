#pragma once

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
    /**
    * @class GLFWManager
    * @author Riko Ophorst
    * @brief Manages all GLFW state such as global initialization / termination, event polling, etc.
    */
    class GLFWManager
    {
        friend class BlowboxCore;
    protected:
        /**
        * @brief Constructor for the GLFWManager. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        */
        GLFWManager();
        /**
        * @brief Destructor for the GLFWManager. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        */
        ~GLFWManager();

        /**
        * @brief Initializes the GLFW environment. This is to be called as one of the first steps in BlowboxCore::Run().
        * @author Riko Ophorst
        */
        void Init();

        /**
        * @brief Updates the GLFW environment. This is to be called every frame.
        * @author Riko Ophorst
        */
        void Update();

        /**
        * @brief Shuts down the GLFW environment. This is to be called as one of the last steps in BlowboxCore::Run() after shutdown has been initiated.
        * @author Riko Ophorst
        */
        void Shutdown();
    };
}