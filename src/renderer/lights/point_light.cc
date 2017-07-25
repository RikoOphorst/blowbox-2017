#include "point_light.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    PointLight::PointLight() :
        name_("PointLight"),
        position_(0.0f, 0.0f, 0.0f),
        color_(1.0f, 1.0f, 1.0f),
        intensity_(1.0f),
        range_(100.0f),
        buffer_dirty_(true)
    {

    }

    //------------------------------------------------------------------------------------------------------
    PointLight::~PointLight()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void PointLight::SetName(const String& name)
    {
        name_ = name;
    }

    //------------------------------------------------------------------------------------------------------
    void PointLight::SetPosition(const DirectX::XMFLOAT3& position)
    {
        position_ = position;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void PointLight::SetColor(const DirectX::XMFLOAT3& color)
    {
        color_ = color;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void PointLight::SetIntensity(float intensity)
    {
        intensity_ = intensity;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void PointLight::SetRange(float range)
    {
        range_ = range;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    const String& PointLight::GetName() const
    {
        return name_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3 & PointLight::GetPosition() const
    {
        return position_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& PointLight::GetColor() const
    {
        return color_;
    }

    //------------------------------------------------------------------------------------------------------
    float PointLight::GetIntensity() const
    {
        return intensity_;
    }

    //------------------------------------------------------------------------------------------------------
    float PointLight::GetRange() const
    {
        return range_;
    }

    //------------------------------------------------------------------------------------------------------
    const PointLight::Buffer& PointLight::GetBuffer()
    {
        if (buffer_dirty_)
        {
            buffer_.color = color_;
            buffer_.intensity = intensity_;
            buffer_.position = position_;
            buffer_.range = range_;
            buffer_.active = true;

            buffer_dirty_ = false;
        }

        return buffer_;
    }
}