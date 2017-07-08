#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32

#include "core/eastl_required.h"
#include <EASTL/string.h>
#include <EASTL/tuple.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "util/resolution.h"

#include <DirectXMath.h>

class GLFWwindow;

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
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
        * @brief Constructs a window. Resolution and window title are required. Window icon is optional.
        * @author Riko Ophorst
        * @param[in] resolution     The resolution of the usable client area
        * @param[in] window_title   The title of the window
        * @param[in] window_icon    The icon that should be used by this window. Usable formats include 16x16, 32x32, 48x48. Optional.
        */
        Window(const Resolution& resolution, const eastl::string& window_title, Image* window_icon = nullptr);

        /**
        * @brief Destructs the window. Note that this does NOT delete the window icon that might have been set. That is your responsibility.
        */
        ~Window();

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
        void SetWindowTitle(const eastl::string& window_title);

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
        * @brief Returns the resolution of the window's client area
        * @author Riko Ophorst
        */
        Resolution GetWindowResolution() const;

        /**
        * @brief Returns the title of the window
        * @author Riko Ophorst
        */
        eastl::string GetWindowTitle() const;

        /**
        * @brief Returns the size limits of the window as a tuple. First is the minimum, second is the maximum.
        * @author Riko Ophorst
        */
        eastl::tuple<Resolution, Resolution> GetWindowSizeLimits() const;

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
        * @brief Returns this window's HWND (handle)
        * @author Riko Ophorst
        */
        HWND GetWindowHandle() const;

    private:
        GLFWwindow* window_; //!< The underlying window that GLFW uses
        Image* window_icon_; //!< The currently bound icon
        eastl::string window_title_; //!< The title of window
        Resolution min_resolution_; //!< The minimum resolution of the window
        Resolution max_resolution_; //!< The maximum resolution of the window
    };
}