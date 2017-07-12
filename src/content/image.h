#pragma once

#include "util/resolution.h"
#include "core/eastl.h"
#include <EASTL/string.h>

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
    /**
    * @enum blowbox::PixelComposition
    * @author Riko Ophorst
    * @brief An enumeration of all different pixel compositions
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
    * @class blowbox::Image
    * @author Riko Ophorst
    * @brief This class enables you to load images from disk
    */
    class Image
    {
    public:
        /**
        * @brief Constructs an Image object. It will load the image given in the image_file_path.
        * @author Riko Ophorst
        * @param[in] image_file_path    The file path to the image you want to be loaded
        */
        Image(const eastl::string& image_file_path);
        
        /**
        * @brief Destructs the Image object.
        * @author Riko Ophorst
        */
        ~Image();

        /**
        * @brief Returns the pixel data that is contained within the image. If you want to know how the pixels are composed, use Image::GetPixelComposition()
        * @author Riko Ophorst
        */
        unsigned char* GetPixelData() const;

        /**
        * @brief Returns the resolution of the image.
        * @author Riko Ophorst
        */
        const Resolution& GetResolution() const;

        /**
        * @brief Returns the pixel composition of the image. Every pixel is composed of a number of channels. This function helps find out what that is.
        * @author Riko Ophorst
        */
        const PixelComposition& GetPixelComposition() const;

    private:
        Resolution resolution_; //!< The resolution of the image
        unsigned char* pixel_data_; //!< The pixel data of this image
        PixelComposition pixel_composition_; //!< The pixel composition of this image
    };
}