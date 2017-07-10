#include "window.h"

#include "core/get.h"
#include "win32/glfw_manager.h"
#include "content/image.h"
#include "util/assert.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Window::Window() :
        window_(nullptr),
        window_icon_(nullptr)
    {
        
    }

    //------------------------------------------------------------------------------------------------------
    Window::~Window()
    {
        Get::GLFWManager()->RemoveWindow(this);
        glfwDestroyWindow(window_);
    }

    //------------------------------------------------------------------------------------------------------
    void Window::Create(const Resolution& resolution, const eastl::string& window_title, Image* window_icon)
    {
        window_ = glfwCreateWindow(resolution.width, resolution.height, window_title.c_str(), nullptr, nullptr);
        BLOWBOX_ASSERT(window_ != nullptr);

        if (window_icon != nullptr)
        {
            SetWindowIcon(window_icon);
        }

        Get::GLFWManager()->AddWindow(this);

        glfwSetKeyCallback(window_, GlfwKeyCallback);
        glfwSetCursorPosCallback(window_, GlfwCursorPosCallback);
        glfwSetMouseButtonCallback(window_, GlfwMouseButtonCallback);
        glfwSetScrollCallback(window_, GlfwScrollCallback);
        glfwSetCursorEnterCallback(window_, GlfwMouseEnterCallback);
        glfwSetWindowFocusCallback(window_, GlfwFocusCallback);

        Focus();
    }

    //------------------------------------------------------------------------------------------------------
    void Window::SetWindowResolution(const Resolution& resolution)
    {
        glfwSetWindowSize(window_, resolution.width, resolution.height);
    }

    //------------------------------------------------------------------------------------------------------
    void Window::SetWindowTitle(const eastl::string& window_title)
    {
        window_title_ = window_title;
        glfwSetWindowTitle(window_, window_title.c_str());
    }

    //------------------------------------------------------------------------------------------------------
    void Window::SetWindowSizeLimits(const Resolution& min_resolution, const Resolution& max_resolution)
    {
        min_resolution_ = min_resolution;
        max_resolution_ = max_resolution;
        glfwSetWindowSizeLimits(window_, min_resolution.width, min_resolution.height, max_resolution.width, max_resolution.height);
    }

    //------------------------------------------------------------------------------------------------------
    void Window::SetWindowPosition(const DirectX::XMINT2& window_position)
    {
        glfwSetWindowPos(window_, window_position.x, window_position.y);
    }

    //------------------------------------------------------------------------------------------------------
    void Window::SetWindowIcon(Image* window_icon)
    {
        window_icon_ = window_icon;

        GLFWimage icon;
        icon.width = window_icon->GetResolution().width;
        icon.height = window_icon->GetResolution().height;
        icon.pixels = window_icon->GetPixelData();

        glfwSetWindowIcon(window_, 1, &icon);
    }

    //------------------------------------------------------------------------------------------------------
    void Window::Focus()
    {
        glfwFocusWindow(window_);
    }

    //------------------------------------------------------------------------------------------------------
    Resolution Window::GetWindowResolution() const
    {
        Resolution resolution;
        glfwGetWindowSize(window_, &resolution.width, &resolution.height);
        return resolution;
    }

    //------------------------------------------------------------------------------------------------------
    eastl::string Window::GetWindowTitle() const
    {
        return window_title_;
    }

    //------------------------------------------------------------------------------------------------------
    eastl::tuple<Resolution, Resolution> Window::GetWindowSizeLimits() const
    {
        return eastl::tuple<Resolution, Resolution>(min_resolution_, max_resolution_);
    }

    //------------------------------------------------------------------------------------------------------
    DirectX::XMINT2 Window::GetWindowPosition() const
    {
        DirectX::XMINT2 pos;
        glfwGetWindowPos(window_, &pos.x, &pos.y);
        return pos;
    }

    //------------------------------------------------------------------------------------------------------
    Image* Window::GetWindowIcon() const
    {
        return window_icon_;
    }

    //------------------------------------------------------------------------------------------------------
    bool Window::GetWindowFocused() const
    {
        return focused_;
    }

    //------------------------------------------------------------------------------------------------------
    HWND Window::GetWindowHandle() const
    {
        return glfwGetWin32Window(window_);
    }
    
    //------------------------------------------------------------------------------------------------------
    GLFWwindow* Window::GetGLFWWindow()
    {
        return window_;
    }

    //------------------------------------------------------------------------------------------------------
    KeyboardState& Window::GetKeyboardState()
    {
        return keyboard_state_;
    }

    //------------------------------------------------------------------------------------------------------
    MouseState& Window::GetMouseState()
    {
        return mouse_state_;
    }

    //------------------------------------------------------------------------------------------------------
    bool Window::GetWindowShouldClose()
    {
        return glfwWindowShouldClose(window_) == GLFW_TRUE;
    }

    //------------------------------------------------------------------------------------------------------
    void Window::GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
    {
        KeyboardState& keyboard_state = Get::GLFWManager()->FindCorrespondingWindow(window)->GetKeyboardState();
        
        if (action == GLFW_PRESS)
        {
            keyboard_state.SetKeyPressed(GlfwKeyToBlowboxKeyCode(key));
        }

        if (action == GLFW_RELEASE)
        {
            keyboard_state.SetKeyReleased(GlfwKeyToBlowboxKeyCode(key));
        }
    }

    //------------------------------------------------------------------------------------------------------
    void Window::GlfwCursorPosCallback(GLFWwindow* window, double x, double y)
    {
        MouseState& mouse_state = Get::GLFWManager()->FindCorrespondingWindow(window)->GetMouseState();
        mouse_state.SetMousePosition(DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y)));
    }
    
    //------------------------------------------------------------------------------------------------------
    void Window::GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers)
    {
        MouseState& mouse_state = Get::GLFWManager()->FindCorrespondingWindow(window)->GetMouseState();

        if (action == GLFW_PRESS)
        {
            mouse_state.SetMouseButtonPressed(GlfwMouseButtonToBlowboxMouseButton(button));
        }

        if (action == GLFW_RELEASE)
        {
            mouse_state.SetMouseButtonReleased(GlfwMouseButtonToBlowboxMouseButton(button));
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    void Window::GlfwScrollCallback(GLFWwindow* window, double x, double y)
    {
        MouseState& mouse_state = Get::GLFWManager()->FindCorrespondingWindow(window)->GetMouseState();

        mouse_state.SetScrollDelta(DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y)));
    }
    
    //------------------------------------------------------------------------------------------------------
    void Window::GlfwMouseEnterCallback(GLFWwindow* window, int entered)
    {
        MouseState& mouse_state = Get::GLFWManager()->FindCorrespondingWindow(window)->GetMouseState();

        mouse_state.SetMouseInWindow(entered == 1);
    }
    
    //------------------------------------------------------------------------------------------------------
    void Window::GlfwFocusCallback(GLFWwindow* window, int focused)
    {
        Window* blowbox_window = Get::GLFWManager()->FindCorrespondingWindow(window);
        blowbox_window->focused_ = focused == 1;
    }
}