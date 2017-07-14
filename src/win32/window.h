#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32

#include "util/string.h"
#include "util/tuple.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "win32/key_code.h"
#include "win32/keyboard_state.h"
#include "win32/mouse_state.h"
#include "util/resolution.h"

#include <DirectXMath.h>

struct GLFWwindow;

namespace blowbox
{
    class Image;

    /**
    * @class blowbox::Window
    * @author Riko Ophorst
    * @brief A window is meant to create a window that can be used for rendering. It opens a GUI window, essentially.
    */
    class Window
    {
    public:
        /**
        * @brief Constructor for Window.
        * @author Riko Ophorst
        */
        Window();

        /**
        * @brief Destructs the window. Note that this does NOT delete the window icon that might have been set. That is your responsibility.
        * @author Riko Ophorst
        */
        ~Window();

        /**
        * @brief Creates the window. Resolution and window title are required. Window icon is optional.
        * @author Riko Ophorst
        * @param[in] resolution     The resolution of the usable client area
        * @param[in] window_title   The title of the window
        * @param[in] window_icon    The icon that should be used by this window. Usable formats include 16x16, 32x32, 48x48. Optional.
        */
        void Create(const Resolution& resolution, const String& window_title, Image* window_icon = nullptr);

        /**
        * @brief Sets the resolution of the window
        * @author Riko Ophorst
        * @param[in] resolution     The resolution of the window
        */
        void SetWindowResolution(const Resolution& resolution);

        /**
        * @brief Sets the title of the window
        * @author Riko Ophorst
        * @param[in] window_title   The title of the window
        */
        void SetWindowTitle(const String& window_title);

        /**
        * @brief Sets the size limits of the window
        * @author Riko Ophorst
        * @param[in] min_resolution     The minimum resolution of the window
        * @param[in] max_resolution     The maximum resolution of the window
        */
        void SetWindowSizeLimits(const Resolution& min_resolution, const Resolution& max_resolution);

        /**
        * @brief Sets the position of the window
        * @author Riko Ophorst
        * @param[in] window_position    The position of the window
        */
        void SetWindowPosition(const DirectX::XMINT2& window_position);

        /**
        * @brief Sets the icon of the window
        * @author Riko Ophorst
        * @param[in] window_icon        The icon of the window
        */
        void SetWindowIcon(Image* window_icon);

        /**
        * @brief Makes the window the focus of the input
        * @author Riko Ophorst
        */
        void Focus();

        /**
        * @brief Returns the resolution of the window's client area
        * @author Riko Ophorst
        */
        Resolution GetWindowResolution() const;

        /**
        * @brief Returns the title of the window
        * @author Riko Ophorst
        */
        String GetWindowTitle() const;

        /**
        * @brief Returns the size limits of the window as a tuple. First is the minimum, second is the maximum.
        * @author Riko Ophorst
        */
        Tuple<Resolution, Resolution> GetWindowSizeLimits() const;

        /**
        * @brief Returns the position of the window's top left position
        * @author Riko Ophorst
        */
        DirectX::XMINT2 GetWindowPosition() const;

        /**
        * @brief Returns the icon that is used by this window
        * @author Riko Ophorst
        */
        Image* GetWindowIcon() const;

        /**
        * @brief Returns whether the window is in focus
        * @author Riko Ophorst
        */
        bool GetWindowFocused() const;

        /**
        * @brief Returns this window's HWND (handle)
        * @author Riko Ophorst
        */
        HWND GetWindowHandle() const;

        /**
        * @brief Returns the underlying GLFWwindow
        * @author Riko Ophorst
        */
        GLFWwindow* GetGLFWWindow();

        /**
        * @brief Returns this window's KeyboardState
        * @author Riko Ophorst
        */
        KeyboardState& GetKeyboardState();

        /**
        * @brief Returns this window's MouseState
        * @author Riko Ophorst
        */
        MouseState& GetMouseState();

        /**
        * @brief Returns whether this Window should be closed. Can be triggered by the user.
        * @author Riko Ophorst
        */
        bool GetWindowShouldClose();

        /**
        * @brief This is the callback function that is used for key input
        * @author Riko Ophorst
        * @param[in] window     The window that the callback function is associated with
        * @param[in] key        The key in question
        * @param[in] scancode   Don't know what this is.
        * @param[in] action     What happened with the key in question
        * @param[in] modifiers  Whether any modifiers were active when this key was pressed
        */
        static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);

        /**
        * @brief This is the callback function that is used for mouse movement input
        * @author Riko Ophorst
        * @param[in] window     The window that the callback function is associated with
        * @param[in] x          The position of the cursor on the x-axis
        * @param[in] y          The position of the cursor on the y-axis
        */
        static void GlfwCursorPosCallback(GLFWwindow* window, double x, double y);

        /**
        * @brief This is the callback function that is used for mouse movement input
        * @author Riko Ophorst
        * @param[in] window     The window that the callback function is associated with
        * @param[in] button     The button in question
        * @param[in] action     The action
        * @param[in] modifiers  Whether any modifiers were active when this button was pressed
        */
        static void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers);

        /**
        * @brief This is the callback function that is used for mouse scroll input
        * @author Riko Ophorst
        * @param[in] window     The window that the callback function is associated with
        * @param[in] x          The scroll offset on the x axis
        * @param[in] y          The scroll offset on the y axis
        */
        static void GlfwScrollCallback(GLFWwindow* window, double x, double y);

        /**
        * @brief This is the callback function that is used for when the mouse enters/leaves the window
        * @author Riko Ophorst
        * @param[in] window     The window that the callback function is associated with
        * @param[in] entered    Whether the mouse entered or left the window
        */
        static void GlfwMouseEnterCallback(GLFWwindow* window, int entered);

        /**
        * @brief This is the callback function that is used for when the mouse enters/leaves the window
        * @author Riko Ophorst
        * @param[in] window     The window that the callback function is associated with
        * @param[in] focused    Whether the window is focused or not
        */
        static void GlfwFocusCallback(GLFWwindow* window, int focused);

    private:
        GLFWwindow* window_; //!< The underlying window that GLFW uses
        Image* window_icon_; //!< The currently bound icon
        String window_title_; //!< The title of window
        Resolution min_resolution_; //!< The minimum resolution of the window
        Resolution max_resolution_; //!< The maximum resolution of the window

        KeyboardState keyboard_state_; //!< The state of the keyboard for this window
        MouseState mouse_state_; //!< The state of the mouse for this window

        bool focused_;
    };
}