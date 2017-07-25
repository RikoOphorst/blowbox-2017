#pragma once

#include <DirectXMath.h>
#include "util/string.h"

namespace blowbox
{
    class DirectionalLight
    {
    public:
        struct Buffer
        {
            DirectX::XMFLOAT3 direction;
            float intensity;
            DirectX::XMFLOAT3 color;
            uint32_t active;
        };

        DirectionalLight();
        ~DirectionalLight();

        void SetName(const String& name);
        void SetDirection(const DirectX::XMFLOAT3& direction);
        void SetColor(const DirectX::XMFLOAT3& color);
        void SetIntensity(float intensity);

        const String& GetName() const;
        const DirectX::XMFLOAT3& GetDirection() const;
        const DirectX::XMFLOAT3& GetColor() const;
        float GetIntensity() const;

        const Buffer& GetBuffer();

    private:
        String name_;
        DirectX::XMFLOAT3 direction_;
        DirectX::XMFLOAT3 color_;
        float intensity_;
        Buffer buffer_;
        bool buffer_dirty_;
    };
}