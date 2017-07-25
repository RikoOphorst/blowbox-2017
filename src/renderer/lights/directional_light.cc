#include "directional_light.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    DirectionalLight::DirectionalLight() :
        name_("DirectionalLight"),
        color_(1.0f, 1.0f, 1.0f),
        direction_(0.5f, -1.0f, 0.5f),
        intensity_(1.0f),
        buffer_dirty_(true)
    {

    }

    //------------------------------------------------------------------------------------------------------
    DirectionalLight::~DirectionalLight()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void DirectionalLight::SetName(const String& name)
    {
        name_ = name;
    }

    //------------------------------------------------------------------------------------------------------
    void DirectionalLight::SetDirection(const DirectX::XMFLOAT3& direction)
    {
        direction_ = direction;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void DirectionalLight::SetColor(const DirectX::XMFLOAT3& color)
    {
        color_ = color;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void DirectionalLight::SetIntensity(float intensity)
    {
        intensity_ = intensity;
        buffer_dirty_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    const String& DirectionalLight::GetName() const
    {
        return name_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& DirectionalLight::GetDirection() const
    {
        return direction_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& DirectionalLight::GetColor() const
    {
        return color_;
    }

    //------------------------------------------------------------------------------------------------------
    float DirectionalLight::GetIntensity() const
    {
        return intensity_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectionalLight::Buffer& DirectionalLight::GetBuffer()
    {
        if (buffer_dirty_)
        {
            buffer_.color = color_;
            buffer_.direction = direction_;
            buffer_.intensity = intensity_;
            buffer_.active = true;

            buffer_dirty_ = false;
        }

        return buffer_;
    }
}