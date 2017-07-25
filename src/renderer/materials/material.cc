#include "material.h"

namespace blowbox 
{
    //------------------------------------------------------------------------------------------------------
    Material::Material() :
        name_("DefaultMaterial"),
        color_ambient_(0.0f, 0.0f, 0.0f),
        color_emissive_(0.0f, 0.0f, 0.0f),
        color_diffuse_(0.0f, 0.0f, 0.0f),
        color_specular_(1.0f, 1.0f, 1.0f),
        opacity_(1.0f),
        specular_scale_(1.0f),
        specular_power_(1.0f),
        bump_intensity_(5.0f),
        buffer_created_(false)
    {

    }

    //------------------------------------------------------------------------------------------------------
    Material::~Material()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetName(const String& name)
    {
        name_ = name;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetColorAmbient(const DirectX::XMFLOAT3& color)
    {
        color_ambient_ = color;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetColorEmissive(const DirectX::XMFLOAT3& color)
    {
        color_emissive_ = color;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetColorDiffuse(const DirectX::XMFLOAT3& color)
    {
        color_diffuse_ = color;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetColorSpecular(const DirectX::XMFLOAT3& color)
    {
        color_specular_ = color;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetOpacity(float opacity)
    {
        opacity_ = opacity;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetSpecularScale(float specular_scale)
    {
        specular_scale_ = specular_scale;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetSpecularPower(float specular_power)
    {
        specular_power_ = specular_power;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetBumpIntensity(float bump_intensity)
    {
        bump_intensity_ = bump_intensity;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetTextureAmbient(WeakPtr<Texture> texture)
    {
        texture_ambient_ = texture;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetTextureDiffuse(WeakPtr<Texture> texture)
    {
        texture_diffuse_ = texture;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetTextureEmissive(WeakPtr<Texture> texture)
    {
        texture_emissive_ = texture;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetTextureBump(WeakPtr<Texture> texture)
    {
        texture_bump_ = texture;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetTextureNormal(WeakPtr<Texture> texture)
    {
        texture_normal_ = texture;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetTextureSpecularPower(WeakPtr<Texture> texture)
    {
        texture_specular_power_ = texture;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetTextureSpecular(WeakPtr<Texture> texture)
    {
        texture_specular_ = texture;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::SetTextureOpacity(WeakPtr<Texture> texture)
    {
        texture_opacity_ = texture;
    }

    //------------------------------------------------------------------------------------------------------
    const String& Material::GetName() const
    {
        return name_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& Material::GetColorAmbient() const
    {
        return color_ambient_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& Material::GetColorEmissive() const
    {
        return color_emissive_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& Material::GetColorDiffuse() const
    {
        return color_diffuse_;
    }

    //------------------------------------------------------------------------------------------------------
    const DirectX::XMFLOAT3& Material::GetColorSpecular() const
    {
        return color_specular_;
    }

    //------------------------------------------------------------------------------------------------------
    const float& Material::GetOpacity() const
    {
        return opacity_;
    }

    //------------------------------------------------------------------------------------------------------
    const float& Material::GetSpecularScale() const
    {
        return specular_scale_;
    }

    //------------------------------------------------------------------------------------------------------
    const float& Material::GetSpecularPower() const
    {
        return specular_power_;
    }

    //------------------------------------------------------------------------------------------------------
    const float& Material::GetBumpIntensity() const
    {
        return bump_intensity_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> Material::GetTextureAmbient() const
    {
        return texture_ambient_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> Material::GetTextureDiffuse() const
    {
        return texture_diffuse_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> Material::GetTextureEmissive() const
    {
        return texture_emissive_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> Material::GetTextureBump() const
    {
        return texture_bump_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> Material::GetTextureNormal() const
    {
        return texture_normal_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> Material::GetTextureSpecularPower() const
    {
        return texture_specular_power_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> Material::GetTextureSpecular() const
    {
        return texture_specular_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> Material::GetTextureOpacity() const
    {
        return texture_opacity_;
    }

    //------------------------------------------------------------------------------------------------------
    UploadBuffer& Material::GetConstantBuffer()
    {
        if (!buffer_created_)
        {
            upload_buffer_.Create(L"MaterialBuffer", 1, sizeof(Material::Buffer));
            buffer_created_ = true;
        }

        Material::Buffer material_buffer;

        ToBuffer(&material_buffer);

        upload_buffer_.InsertDataByElement(0, &material_buffer);

        return upload_buffer_;
    }

    //------------------------------------------------------------------------------------------------------
    void Material::ToBuffer(Material::Buffer* output)
    {
        if (output != nullptr)
        {
            output->bump_intensity = bump_intensity_;
            output->color_ambient = color_ambient_;
            output->color_diffuse = color_diffuse_;
            output->color_emissive = color_emissive_;
            output->color_specular = color_specular_;
            output->opacity = opacity_;
            output->specular_scale = specular_scale_;
            output->specular_power = specular_power_;
            output->use_ambient_texture = static_cast<uint32_t>(!texture_ambient_.expired());
            output->use_diffuse_texture = static_cast<uint32_t>(!texture_diffuse_.expired());
            output->use_emissive_texture = static_cast<uint32_t>(!texture_emissive_.expired());
            output->use_bump_texture = static_cast<uint32_t>(!texture_bump_.expired());
            output->use_normal_texture = static_cast<uint32_t>(!texture_normal_.expired());
            output->use_shininess_texture = static_cast<uint32_t>(!texture_specular_power_.expired());
            output->use_specular_texture = static_cast<uint32_t>(!texture_specular_.expired());
            output->use_opacity_texture = static_cast<uint32_t>(!texture_opacity_.expired());
        }
    }
}