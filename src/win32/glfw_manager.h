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
	* @brief Manages all GLFW state
	*/
	class GLFWManager
	{
		friend class BlowboxCore;
	protected:
		GLFWManager();
		~GLFWManager();

		void Init();
		void Update();
		void Shutdown();

	private:
		
	};
}