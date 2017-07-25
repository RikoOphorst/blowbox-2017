#include "image.h"

#include "util/assert.h"

#define STB_IMAGE_IMPLEMENTATION
#include "content/stb/stb_image.h"

#include "core/get.h"
#include "core/debug/console.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Image::Image(const String& image_file_path) :
        pixel_data_(nullptr),
        pixel_composition_(PixelComposition_UNKNOWN),
        image_file_path_(image_file_path),
        corrupt_(false)
    {
        Reload();
    }

    //------------------------------------------------------------------------------------------------------
    Image::~Image()
    {
        if (!corrupt_)
        {
            stbi_image_free(pixel_data_);
        }
        else
        {
            delete[] pixel_data_;
        }
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
    const String& Image::GetFilePath() const
    {
        return image_file_path_;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Image::Reload()
    {
        if (pixel_data_ != nullptr)
        {
            if (!corrupt_)
            {
                stbi_image_free(pixel_data_);
            }
            else
            {
                delete[] pixel_data_;
            }
        }

        struct stat buffer;
        if (stat(image_file_path_.c_str(), &buffer) != 0)
        {
            pixel_data_ = new unsigned char[4];

            pixel_data_[0] = 255u;
            pixel_data_[1] = 0u;
            pixel_data_[2] = 255u;
            pixel_data_[3] = 255u;

            resolution_.width = 1;
            resolution_.height = 1;

            corrupt_ = true;

            char buf[256];
            sprintf(buf, "Tried loading an Image (%s) but the file couldn't be found on disk. Using default image data instead.", image_file_path_.c_str());
            Get::Console()->LogError(buf);
        }
        else
        {
            int pixel_composition;
            pixel_data_ = stbi_load(image_file_path_.c_str(), &resolution_.width, &resolution_.height, &pixel_composition, STBI_rgb_alpha);

            if (pixel_data_ == nullptr)
            {
                pixel_data_ = new unsigned char[4];

                pixel_data_[0] = 255u;
                pixel_data_[1] = 0u;
                pixel_data_[2] = 255u;
                pixel_data_[3] = 255u;

                resolution_.width = 1;
                resolution_.height = 1;

                corrupt_ = true;

                char buf[256];
                sprintf(buf, "An Image (%s) couldn't be loaded from disk because the file type is unsupported or corrupted. Using default image data instead.", image_file_path_.c_str());
                Get::Console()->LogError(buf);
            }
        }

        BLOWBOX_ASSERT(pixel_data_ != nullptr);

        pixel_composition_ = PixelComposition_RGBA;
    }
}