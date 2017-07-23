#include "texture.h"

#include "core/get.h"
#include "core/debug/console.h"
#include "util/shared_ptr.h"
#include "renderer/commands/command_context.h"

#include <locale>
#include <codecvt>

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Texture::Texture()
    {
        buffer_.Create(L"TextureBuffer: Default", 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM);

        unsigned char pixel[4];
        pixel[0] = 255;
        pixel[1] = 0;
        pixel[2] = 255;
        pixel[3] = 255;

        D3D12_SUBRESOURCE_DATA data;
        data.pData = pixel;
        data.RowPitch = 1 * 4;
        data.SlicePitch = data.RowPitch * 1;
        CommandContext::InitializeTexture(buffer_, 1, &data);
    }

    //------------------------------------------------------------------------------------------------------
    Texture::Texture(WeakPtr<Image> image) :
        image_(image)
    {
        Reload();
    }

    //------------------------------------------------------------------------------------------------------
    Texture::~Texture()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    void Texture::Reload()
    {
        if (!image_.expired())
        {
            Reload(image_);
        }
    }

    //------------------------------------------------------------------------------------------------------
    void Texture::Reload(WeakPtr<Image> image)
    {
        image_ = image;

        SharedPtr<Image> image_ptr = image.lock();

        wchar_t buf[512];
        swprintf(buf, L"TextureBuffer");

        int num_channels = 0;
        switch (image_ptr->GetPixelComposition())
        {
        case PixelComposition_R:
            num_channels = 1;
            buffer_.Create(buf, image_ptr->GetResolution().width, image_ptr->GetResolution().height, DXGI_FORMAT_R8_UNORM);
            break;
        case PixelComposition_RG:
            num_channels = 2;
            buffer_.Create(buf, image_ptr->GetResolution().width, image_ptr->GetResolution().height, DXGI_FORMAT_R8G8_UNORM);
            break;
        case PixelComposition_RGB:
            num_channels = 3;
            Get::Console()->LogWarning("Trying to create a Texture with a 24 bit format type, however this is not properly supported. Only 32 bit format types are supported. Using DXGI_FORMAT_R8G8B8A8_UNORM as a format instead.");
            buffer_.Create(buf, image_ptr->GetResolution().width, image_ptr->GetResolution().height, DXGI_FORMAT_R8G8B8A8_UNORM);
            break;
        case PixelComposition_RGBA:
            num_channels = 4;
            buffer_.Create(buf, image_ptr->GetResolution().width, image_ptr->GetResolution().height, DXGI_FORMAT_R8G8B8A8_UNORM);
            break;
        }

        D3D12_SUBRESOURCE_DATA data;
        data.pData = image_ptr->GetPixelData();
        data.RowPitch = image_ptr->GetResolution().width * num_channels;
        data.SlicePitch = data.RowPitch * image_ptr->GetResolution().height;
        CommandContext::InitializeTexture(buffer_, 1, &data);
    }

    //------------------------------------------------------------------------------------------------------
    ColorBuffer& Texture::GetBuffer()
    {
        return buffer_;
    }
    
    //------------------------------------------------------------------------------------------------------
    const String& Texture::GetName() const
    {
        return name_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Image> Texture::GetImage() const
    {
        return image_;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Texture::SetName(const String& name)
    {
        name_ = name;
    }
}