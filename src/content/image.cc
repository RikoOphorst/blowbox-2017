#include "image.h"

#include "util/assert.h"

#define STB_IMAGE_IMPLEMENTATION
#include "content/stb/stb_image.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Image::Image(const String& image_file_path) :
        pixel_data_(nullptr),
        pixel_composition_(PixelComposition_UNKNOWN),
        image_file_path_(image_file_path)
    {
        Reload();
    }

    //------------------------------------------------------------------------------------------------------
    Image::~Image()
    {
        stbi_image_free(pixel_data_);
    }

    //------------------------------------------------------------------------------------------------------
    unsigned char* const Image::GetPixelData() const
    {
        return pixel_data_;
    }

    //------------------------------------------------------------------------------------------------------
    const Resolution& Image::GetResolution() const
    {
        return resolution_;
    }

    //------------------------------------------------------------------------------------------------------
    const PixelComposition& Image::GetPixelComposition() const
    {
        return pixel_composition_;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Image::Reload()
    {
        if (pixel_data_ != nullptr)
        {
            stbi_image_free(pixel_data_);
        }

        int pixel_composition;
        pixel_data_ = stbi_load(image_file_path_.c_str(), &resolution_.width, &resolution_.height, &pixel_composition, STBI_rgb_alpha);

        struct stat buffer;
        BLOWBOX_ASSERT((stat(image_file_path_.c_str(), &buffer) == 0 && pixel_data_ != nullptr));

        pixel_composition_ = static_cast<PixelComposition>(pixel_composition);
    }
}