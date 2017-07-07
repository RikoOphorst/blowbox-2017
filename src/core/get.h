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

	/**
	* @class blowbox::Get
	* @author Riko Ophorst
	* @brief This class is meant to provide simple access to all managers in engine
	*/
	class Get
	{
		friend class BlowboxCore;
	protected:
		Get();
		~Get();

	public:
		static BlowboxCore* BlowboxCore();
		static GLFWManager* GLFWManager();
		static Window* MainWindow();
		
	protected:
		void SetBlowboxCore(blowbox::BlowboxCore* blowbox_core);
		void SetGLFWManager(blowbox::GLFWManager* glfw_manager);
		void SetMainWindow(blowbox::Window* main_window);

		static Get* instance_;

	private:
		blowbox::BlowboxCore* blowbox_core_;
		blowbox::GLFWManager* glfw_manager_;
		blowbox::Window* main_window_;
	};
}