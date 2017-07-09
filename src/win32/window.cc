#include "window.h"

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
    HWND Window::GetWindowHandle() const
    {
        return glfwGetWin32Window(window_);
    }
}