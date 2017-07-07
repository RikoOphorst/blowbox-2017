#pragma once

#include "util/resolution.h"
#include "core/eastl_required.h"
#include <EASTL/string.h>

namespace blowbox
{
	enum PixelComposition
	{
		PixelComposition_R,
		PixelComposition_RG,
		PixelComposition_RGB,
		PixelComposition_RGBA,
		PixelComposition_UNKNOWN
	};

	class Image
	{
	public:
		Image(const eastl::string& image_file_path);
		~Image();

		unsigned char* GetPixelData();
		const Resolution& GetResolution();

	private:
		Resolution resolution_;
		unsigned char* pixel_data_;
		PixelComposition pixel_composition_;
	};
}