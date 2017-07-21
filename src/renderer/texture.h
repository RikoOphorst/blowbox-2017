#pragma once

#include "util/weak_ptr.h"
#include "util/string.h"
#include "content/image.h"
#include "renderer/buffers/color_buffer.h"

namespace blowbox
{
    class Texture
    {
    public:
        Texture();
        Texture(WeakPtr<Image> image);
        ~Texture();

        void Reload();
        void Reload(WeakPtr<Image> image);

        ColorBuffer& GetBuffer();
        const String& GetName() const;

        void SetName(const String& name);
    private:
        String name_;
        WeakPtr<Image> image_;
        ColorBuffer buffer_;
    };
}