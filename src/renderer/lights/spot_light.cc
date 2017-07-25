#include "spot_light.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    SpotLight::SpotLight() :
        name_("SpotLight"),
        position_(0.0f, 0.0f, 0.0f),
        color_(1.0f, 1.0f, 1.0f),
        direction_(0.5f, -1.0f, 0.5f),
        intensity_(1.0f),
        range_(100.0f),
        spot_angle_(0.785f),
        buffer_dirty_(true)
    {

    }

    //------------------------------------------------------------------------------------------------------
    SpotLight::~SpotLight()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void SpotLight::SetName(const String& name)
    {
        name_ = name;
    }

    //------------------------------------------------------------------------------------------------------
    void SpotLight::SetPosition(const DirectX::XMFLOAT3& position)
    {
        position_ = position;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void SpotLight::SetDirection(const DirectX::XMFLOAT3& direction)
    {
        direction_ = direction;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void SpotLight::SetColor(const DirectX::XMFLOAT3& color)
    {
        color_ = color;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void SpotLight::SetIntensity(float intensity)
    {
        intensity_ = intensity;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void SpotLight::SetRange(float range)
    {
        range_ = range;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void SpotLight::SetSpotAngle(float angle)
    {
        spot_angle_ = angle;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    const String& SpotLight::GetName() const
    {
        return name_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& SpotLight::GetPosition() const
    {
        return position_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& SpotLight::GetDirection() const
    {
        return direction_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& SpotLight::GetColor() const
    {
        return color_;
    }

    //------------------------------------------------------------------------------------------------------
    float SpotLight::GetIntensity() const
    {
        return intensity_;
    }

    //------------------------------------------------------------------------------------------------------
    float SpotLight::GetRange() const
    {
        return range_;
    }

    //------------------------------------------------------------------------------------------------------
    float SpotLight::GetSpotAngle() const
    {
        return spot_angle_;
    }

    //------------------------------------------------------------------------------------------------------
    const SpotLight::Buffer& SpotLight::GetBuffer()
    {
        if (buffer_dirty_)
        {
            buffer_.color = color_;
            buffer_.direction = direction_;
            buffer_.intensity = intensity_;
            buffer_.position = position_;
            buffer_.range = range_;
            buffer_.spot_angle = spot_angle_;

            buffer_dirty_ = false;
        }

        return buffer_;
    }
}