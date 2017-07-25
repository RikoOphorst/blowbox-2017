#pragma once

#include <DirectXMath.h>
#include "util/string.h"

namespace blowbox
{
    class PointLight
    {
    public:
        struct Buffer
        {
            DirectX::XMFLOAT3 position;
            float range;
            DirectX::XMFLOAT3 color;
            float intensity;
            uint32_t active;
            float padding[2];
        };

        PointLight();
        ~PointLight();

        void SetName(const String& name);
        void SetPosition(const DirectX::XMFLOAT3& position);
        void SetColor(const DirectX::XMFLOAT3& color);
        void SetIntensity(float intensity);
        void SetRange(float range);

        const String& GetName() const;
        const DirectX::XMFLOAT3& GetPosition() const;
        const DirectX::XMFLOAT3& GetColor() const;
        float GetIntensity() const;
        float GetRange() const;

        const Buffer& GetBuffer();

    private:
        String name_;
        DirectX::XMFLOAT3 position_;
        DirectX::XMFLOAT3 color_;
        float intensity_;
        float range_;
        Buffer buffer_;
        bool buffer_dirty_;
    };
}