#pragma once

#include "util/map.h"

struct GLFWwindow;

namespace blowbox
{
    class Window;

    /**
    * This class manages all GLFW state. Normally, you would probably not need to interact with this class.
    *
    * @brief Manages all GLFW state such as global initialization / termination, event polling, etc.
    */
    class GLFWManager
    {
        friend class BlowboxCore;
        friend class Window;
	public:
        /**
        * @brief Constructor for the GLFWManager.
        * @author Riko Ophorst
        */
        GLFWManager();

		/**
        * @brief Destructor for the GLFWManager.
        * @author Riko Ophorst
        */
        ~GLFWManager();

	protected:
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

        /**
        * @brief Adds a Window to the GLFWManager.
        * @author Riko Ophorst
        * @param[in] window The window to be added
        */
        void AddWindow(Window* window);

        /**
        * @brief Removes a Window from the GLFWManager.
        * @author Riko Ophorst
        * @param[in] window The window to be removed
        */
        void RemoveWindow(Window* window);

        /**
        * @brief Finds corresponding Window
        * @author Riko Ophorst
        * @param[in] window The GLFW window to which we need to find the Window
        */
        Window* FindCorrespondingWindow(GLFWwindow* window);
    private:
        Map<GLFWwindow*, Window*> windows_; //!< All windows in the world
    };
}