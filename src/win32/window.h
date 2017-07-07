#pragma once

#include "core/eastl_required.h"
#include <EASTL/string.h>

#include "util/resolution.h"

#include <DirectXMath.h>

class GLFWwindow;

namespace blowbox
{
	class Image;

	class Window
	{
	public:
		Window(const Resolution& resolution, const eastl::string& window_title);
		~Window();

		void SetWindowResolution(const Resolution& resolution);
		void SetWindowTitle(const eastl::string& window_title);
		void SetWindowSizeLimits(const Resolution& min_resolution, const Resolution& max_resolution);
		void SetWindowPosition(const DirectX::XMFLOAT2& window_position);
		void SetWindowIcon(Image* image);

	private:
		GLFWwindow* window_;
		Image* window_icon_;
	};
}