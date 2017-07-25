#pragma once

#include <DirectXMath.h>
#include "util/string.h"

namespace blowbox
{
    class SpotLight
    {
    public:
        struct Buffer
        {
            DirectX::XMFLOAT3 position;
            float range;
            DirectX::XMFLOAT3 direction;
            float intensity;
            DirectX::XMFLOAT3 color;
            float spot_angle;
            uint32_t active;
            float padding[3];
        };

        SpotLight();
        ~SpotLight();
        
        void SetName(const String& name);
        void SetPosition(const DirectX::XMFLOAT3& position);
        void SetDirection(const DirectX::XMFLOAT3& direction);
        void SetColor(const DirectX::XMFLOAT3& color);
        void SetIntensity(float intensity);
        void SetRange(float range);
        void SetSpotAngle(float angle);

        const String& GetName() const;
        const DirectX::XMFLOAT3& GetPosition() const;
        const DirectX::XMFLOAT3& GetDirection() const;
        const DirectX::XMFLOAT3& GetColor() const;
        float GetIntensity() const;
        float GetRange() const;
        float GetSpotAngle() const;

        const Buffer& GetBuffer();

    private:
        String name_;
        DirectX::XMFLOAT3 position_;
        DirectX::XMFLOAT3 direction_;
        DirectX::XMFLOAT3 color_;
        float intensity_;
        float range_;
        float spot_angle_;
        Buffer buffer_;
        bool buffer_dirty_;
    };
}