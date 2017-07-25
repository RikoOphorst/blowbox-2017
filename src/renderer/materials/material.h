#pragma once

#include "renderer/d3d12_includes.h"
#include "util/string.h"
#include "util/weak_ptr.h"
#include "renderer/textures/texture.h"
#include "renderer/buffers/upload_buffer.h"

namespace blowbox
{
    /**
    * This class is pretty straightforward. If you want to determine how a Mesh is
    * rendered, you use this class. If you want to convert the Material to a layout
    * that is usable as a constant buffer, use the Material::ToBuffer() function.
    *
    * @brief Describes a Material that can be applied to Meshes during rendering.
    */
    class Material
    {
    public:
        struct Buffer
        {
            DirectX::XMFLOAT3 color_diffuse;
            uint32_t use_ambient_texture;       // 16 byte boundary
            DirectX::XMFLOAT3 color_specular;
            uint32_t use_diffuse_texture;       // 16 byte boundary
            DirectX::XMFLOAT3 color_ambient;
            uint32_t use_emissive_texture;      // 16 byte boundary
            DirectX::XMFLOAT3 color_emissive;
            uint32_t use_bump_texture;          // 16 byte boundary

            float opacity;
            float specular_scale;
            float specular_power;
            float bump_intensity;               // 16 byte boundary

            uint32_t use_normal_texture;
            uint32_t use_shininess_texture;
            uint32_t use_specular_texture;
            uint32_t use_opacity_texture;       // 16 byte boundary
        };

        Material();
        ~Material();

        /**
        * @brief Sets the name of this Material.
        * @param[in] name The new name.
        * @remarks Remarkably, this does not update the entry in the MaterialManager. Keep that in mind.
        */
        void SetName(const String& name);

        /**
        * @brief Sets this Material's ambient color (used if there is no ambient texture available).
        * @param[in] color The new ambient color.
        */
        void SetColorAmbient(const DirectX::XMFLOAT3& color);

        /**
        * @brief Sets this Material's emissive color (used if there is no emissive texture available).
        * @param[in] color The new emissive color.
        */
        void SetColorEmissive(const DirectX::XMFLOAT3& color);

        /**
        * @brief Sets this Material's diffuse color (used if there is no diffuse texture available).
        * @param[in] color The new diffuse color.
        */
        void SetColorDiffuse(const DirectX::XMFLOAT3& color);

        /**
        * @brief Sets this Material's specular color (used if there is no specular texture available).
        * @param[in] color The new specular color.
        */
        void SetColorSpecular(const DirectX::XMFLOAT3& color);

        /**
        * @brief Sets this Material's opacity. This gets applied globally over the Mesh that is being rendered with this Material.
        * @param[in] opacity The new opacity value.
        */
        void SetOpacity(float opacity);

        /**
        * @brief Sets the specular scale of this Material. It scales the specular color.
        * @param[in] specular_scale The new shininess value.
        */
        void SetSpecularScale(float specular_scale);

        /**
        * @brief Sets the specular power. This determines how shiny the object actually is (sometimes referred to as shininess). Used only if no specular power texture is available.
        * @param[in] specular_power The new shininess strength value.
        */
        void SetSpecularPower(float specular_power);

        /**
        * @brief Sets the bump intensity. This is used for bump mapping. It basically determines how far the bump values (which are in the range [0-1]) should be scaled. 
        * @param[in] bump_intensity The new bump intensity value.
        */
        void SetBumpIntensity(float bump_intensity);

        /**
        * @brief Sets the ambient Texture for this Material. This should be an RGB texture.
        * @param[in] texture The texture that should be used as the ambient texture.
        */
        void SetTextureAmbient(WeakPtr<Texture> texture);

        /**
        * @brief Sets the diffuse Texture for this Material. This should be an RGB texture.
        * @param[in] texture The texture that should be used as the diffuse texture.
        */
        void SetTextureDiffuse(WeakPtr<Texture> texture);

        /**
        * @brief Sets the emissive Texture for this Material. This should be an RGB texture.
        * @param[in] texture The texture that should be used as the emissive texture.
        */
        void SetTextureEmissive(WeakPtr<Texture> texture);

        /**
        * @brief Sets the bump Texture for this Material. This should be an texture with only a single gray channel.
        * @param[in] texture The texture that should be used as the bump texture.
        */
        void SetTextureBump(WeakPtr<Texture> texture);

        /**
        * @brief Sets the normal Texture for this Material. This should be an RGB texture.
        * @param[in] texture The texture that should be used as the normal texture.
        */
        void SetTextureNormal(WeakPtr<Texture> texture);

        /**
        * @brief Sets the specular power Texture for this Material. This should be an RGB texture.
        * @param[in] texture The texture that should be used as the specular power texture.
        */
        void SetTextureSpecularPower(WeakPtr<Texture> texture);

        /**
        * @brief Sets the ambient Texture for this Material. This should be an RGB texture.
        * @param[in] texture The texture that should be used as the specular texture.
        */
        void SetTextureSpecular(WeakPtr<Texture> texture);

        /**
        * @brief Sets the opacity Texture for this Material. This should be a single channel texture.
        * @param[in] texture The texture that should be used as the opacity texture.
        */
        void SetTextureOpacity(WeakPtr<Texture> texture);

        /** @returns This material's name. */
        const String& GetName() const;

        /** @returns This material's ambient color. */
        const DirectX::XMFLOAT3& GetColorAmbient() const;
        /** @returns This material's emissive color. */
        const DirectX::XMFLOAT3& GetColorEmissive() const;
        /** @returns This material's diffuse color. */
        const DirectX::XMFLOAT3& GetColorDiffuse() const;
        /** @returns This material's specular color. */
        const DirectX::XMFLOAT3& GetColorSpecular() const;

        /** @returns This material's opacity. */
        const float& GetOpacity() const;
        /** @returns This material's shininess. */
        const float& GetSpecularScale() const;
        /** @returns This material's shininess strength. */
        const float& GetSpecularPower() const;
        /** @returns This material's bump intensity. */
        const float& GetBumpIntensity() const;

        /** @returns This material's ambient texture map. */
        WeakPtr<Texture> GetTextureAmbient() const;
        /** @returns This material's diffuse texture map. */
        WeakPtr<Texture> GetTextureDiffuse() const;
        /** @returns This material's emissive texture map. */
        WeakPtr<Texture> GetTextureEmissive() const;
        /** @returns This material's bump texture map. */
        WeakPtr<Texture> GetTextureBump() const;
        /** @returns This material's normal texture map. */
        WeakPtr<Texture> GetTextureNormal() const;
        /** @returns This material's specular power texture map. */
        WeakPtr<Texture> GetTextureSpecularPower() const;
        /** @returns This material's specular texture map. */
        WeakPtr<Texture> GetTextureSpecular() const;
        /** @returns This material's opacity texture map. */
        WeakPtr<Texture> GetTextureOpacity() const;

        /** @returns The constant buffer that this Material uses. It has already been filled out. */
        UploadBuffer& GetConstantBuffer();

    protected:
        /**
        * @brief Converts this Material to a Material::Buffer that is readable by the GPU.
        * @param[out] output Pointer to where the buffer should be outputted. Can be nullptr.
        */
        void ToBuffer(Material::Buffer* output);

    private:
        String name_;                               //!< The name of this Material.

        DirectX::XMFLOAT3 color_ambient_;           //!< The ambient color of this Material.
        DirectX::XMFLOAT3 color_emissive_;          //!< The emissive color of this Material.
        DirectX::XMFLOAT3 color_diffuse_;           //!< The diffuse color of this Material.
        DirectX::XMFLOAT3 color_specular_;          //!< The specular color of this Material.

        float opacity_;                             //!< The opacity of this Material.
        float specular_scale_;                      //!< The shininess of this Material (this scales the specular color).
        float specular_power_;                      //!< The shininess strength of this Material (defines how shiny the object is).
        float bump_intensity_;                      //!< The bump intensity of this Material (defines how "far" bumps in bump maps should be scaled).

        WeakPtr<Texture> texture_ambient_;          //!< The ambient texture map of this Material.          (RGB32)
        WeakPtr<Texture> texture_diffuse_;          //!< The diffuse texture map of this Material.          (RGB32)
        WeakPtr<Texture> texture_emissive_;         //!< The emissive texture map of this Material.         (RGB32)
        WeakPtr<Texture> texture_bump_;             //!< The bump texture map of this Material.             (R8)
        WeakPtr<Texture> texture_normal_;           //!< The normal texture map of this Material.           (RGB32)
        WeakPtr<Texture> texture_specular_power_;   //!< The specular power texture map of this Material.   (R8)
        WeakPtr<Texture> texture_specular_;         //!< The specular texture map of this Material.         (RGB32)
        WeakPtr<Texture> texture_opacity_;          //!< The opacity texture map of this Material.          (R8)

        UploadBuffer upload_buffer_;                //!< The buffer used to upload the material data to the GPU.
        bool buffer_created_;                       //!< Whether the upload buffer has been created.
    };
}