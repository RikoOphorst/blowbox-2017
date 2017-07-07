#include "window.h"

#include <GLFW/glfw3.h>
#include "content/image.h"
#include "util/assert.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	Window::Window(const Resolution& resolution, const eastl::string& window_title) :
		window_(nullptr),
		window_icon_(nullptr)
	{
		window_ = glfwCreateWindow(resolution.width, resolution.height, window_title.c_str(), nullptr, nullptr);
		BLOWBOX_ASSERT(window_ != nullptr);
	}

	//------------------------------------------------------------------------------------------------------
	Window::~Window()
	{
		glfwDestroyWindow(window_);
	}

	//------------------------------------------------------------------------------------------------------
	void Window::SetWindowResolution(const Resolution& resolution)
	{
		glfwSetWindowSize(window_, resolution.width, resolution.height);
	}

	//------------------------------------------------------------------------------------------------------
	void Window::SetWindowTitle(const eastl::string& window_title)
	{
		glfwSetWindowTitle(window_, window_title.c_str());
	}

	//------------------------------------------------------------------------------------------------------
	void Window::SetWindowSizeLimits(const Resolution& min_resolution, const Resolution& max_resolution)
	{
		glfwSetWindowSizeLimits(window_, min_resolution.width, min_resolution.height, max_resolution.width, max_resolution.height);
	}

	//------------------------------------------------------------------------------------------------------
	void Window::SetWindowPosition(const DirectX::XMFLOAT2& window_position)
	{
		glfwSetWindowPos(window_, window_position.x, window_position.y);
	}

	//------------------------------------------------------------------------------------------------------
	void Window::SetWindowIcon(Image* image)
	{
		window_icon_ = image;

		GLFWimage icon;
		icon.width = image->GetResolution().width;
		icon.height = image->GetResolution().height;
		icon.pixels = image->GetPixelData();

		glfwSetWindowIcon(window_, 1, &icon);
	}
}