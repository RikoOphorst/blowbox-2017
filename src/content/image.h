#pragma once

#include "util/resolution.h"
#include "util/string.h"

namespace blowbox
{
    /**
    * When requesting pixel data from a blowbox::Image, you get an array of pixels.
    * The pixels are composed of a number of components that together form the per 
    * pixel data. Each component is an unsigned char (8 bits). This enumeration is
    * used to determine what the components are per pixel. Remember, all components
    * are 8 bits (1 byte / 1 unsigned char). So if the PixelComposition is
    * PixelComposition_RGB, that means that each pixel takes up three bytes: the red 
    * channel (R), the green channel (G) and blue channel (B) - in that order.
    *
    * @brief An enumeration of all different pixel compositions.
    */
    enum PixelComposition
    {
        PixelComposition_R,
        PixelComposition_RG,
        PixelComposition_RGB,
        PixelComposition_RGBA,
        PixelComposition_UNKNOWN
    };

    /**
    * blowbox::Image lets you easily load images that are stored on disk based on a file path that you provide.
    * Supplied file paths are relative to the application's current working directory. The class provides accessor
    * functions for the underlying pixel data. All data is stored on the CPU, no GPU is involved in the Image
    * class whatsoever. Under the hood, stb_image is used to load the images into RAM.
    *
    * @brief Load and access image files with the use of blowbox::Image.
    */
    class Image
    {
    public:
        /**
        * @brief Constructs an Image object by loading it from disk.
        * @param[in] image_file_path    The file path to the image you want to be loaded.
        */
        Image(const String& image_file_path);
        
        /**
        * This destroys the entire Image object and cleans up all data inside of it. Make sure nobody depends on this Image anymore.
        * @brief Destructs the Image object.
        */
        ~Image();

        /**
        * @brief Returns the pixel data that is contained within the image. If you want to know how the pixels are composed, use Image::GetPixelComposition().
        * @returns This returns an unsigned char* which acts as an array of pixels which is the size of the image width * image height * components_per_pixel. Do not delete this pointer, it is owned by the Image instance.
        * @remarks For an explanation on how to read the data in this pointer, refer to blowbox::PixelComposition.
        */
        unsigned char* const GetPixelData() const;

        /**
        * @brief Returns the resolution of the image.
        * @returns The resolution of the image.
        */
        const Resolution& GetResolution() const;

        /**
        * @brief Returns the pixel composition of the image. Every pixel is composed of a number of channels. This function helps find out what that is.
        * @returns The composition of a single pixel. Refer to blowbox::PixelComposition for more information.
        */
        const PixelComposition& GetPixelComposition() const;

        /** @returns The file path to where this Image is stored on disk. */
        const String& GetFilePath() const;

        /** @brief Reloads the Image. */
        void Reload();

    private:
        String image_file_path_; //!< The file path that links to the image that was used to load this image from disk.
        Resolution resolution_; //!< The resolution of the image is stored here.
        unsigned char* pixel_data_; //!< The actual pixel data of the image. This pointer is owned, generated and destroyed by stb_image. We are only allowed to read from it.
        PixelComposition pixel_composition_; //!< The per pixel composition of the pixel data. Refer to blowbox::PixelComposition.
    };
}