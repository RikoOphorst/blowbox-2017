#pragma once

#include "util/weak_ptr.h"
#include "util/string.h"
#include "content/image.h"
#include "renderer/buffers/color_buffer.h"

namespace blowbox
{
    /**
    * @brief Essentially the same as an Image, except the Texture also creates render-buffer.
    */
    class Texture
    {
    public:
        /** @brief Constructs a default Texture (1 by 1, \#FF00FF) */
        Texture();

        /**
        * @brief Construct a Texture based on Image data.
        * @param[in] image The image to base this Texture on.
        */
        Texture(WeakPtr<Image> image);
        ~Texture();

        /** @brief Reloads this Texture based on the Image that it was created with.. */
        void Reload();

        /** 
        * @brief Reloads this Texture based on an Image that is passed in.
        * @param[in] image The image to base this Texture on.
        */
        void Reload(WeakPtr<Image> image);

        /** @returns The underlying ColorBuffer. */
        ColorBuffer& GetBuffer();

        /** @returns The name of this Texture (usually based on the Image name). */
        const String& GetName() const;

        /** 
        * @brief Sets the name of this Texture.
        * @param[in] name The new name of this Texture. 
        */
        void SetName(const String& name);

        /** @returns The Image that this Texture is based on. */
        WeakPtr<Image> GetImage() const;
    private:
        String name_;           //!< Name of this Texture.
        WeakPtr<Image> image_;  //!< The Image this Texture is based on.
        ColorBuffer buffer_;    //!< The ColorBuffer containing the Image data.
    };
}