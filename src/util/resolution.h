#pragma once

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
	/**
	* @struct blowbox::Resolution
	* @author Riko Ophorst
	* @brief Stores resolution information in pixels
	*/
	struct Resolution
	{
		Resolution();
		Resolution(int width, int height);

		int width;
		int height;

		float GetAspectRatio();
	};
}