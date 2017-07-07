#include "resolution.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	Resolution::Resolution() :
		width(0),
		height(0)
	{

	}

	//------------------------------------------------------------------------------------------------------
	Resolution::Resolution(int width, int height) :
		width(width),
		height(height)
	{

	}

	//------------------------------------------------------------------------------------------------------
	float Resolution::GetAspectRatio()
	{
		return static_cast<float>(width) / static_cast<float>(height);
	}
}