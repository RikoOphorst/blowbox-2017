#include "get.h"

#include "util/assert.h"

namespace blowbox
{
	Get* Get::instance_ = nullptr;

	//------------------------------------------------------------------------------------------------------
	Get::Get() :
		blowbox_core_(nullptr),
		glfw_manager_(nullptr),
		main_window_(nullptr)
	{
		BLOWBOX_ASSERT(instance_ == nullptr);
		instance_ = this;
	}

	//------------------------------------------------------------------------------------------------------
	Get::~Get()
	{
		instance_ = nullptr;
	}

	//------------------------------------------------------------------------------------------------------
	BlowboxCore* Get::BlowboxCore()
	{
		return Get::instance_->blowbox_core_;
	}

	//------------------------------------------------------------------------------------------------------
	GLFWManager* Get::GLFWManager()
	{
		return Get::instance_->glfw_manager_;
	}

	//------------------------------------------------------------------------------------------------------
	Window* Get::MainWindow()
	{
		return Get::instance_->main_window_;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Get::SetBlowboxCore(blowbox::BlowboxCore* blowbox_core)
	{
		blowbox_core_ = blowbox_core;
	}

	//------------------------------------------------------------------------------------------------------
	void Get::SetGLFWManager(blowbox::GLFWManager* glfw_manager)
	{
		glfw_manager_ = glfw_manager;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Get::SetMainWindow(blowbox::Window* main_window)
	{
		main_window_ = main_window;
	}
}