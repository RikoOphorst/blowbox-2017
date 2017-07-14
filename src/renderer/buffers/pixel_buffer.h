#pragma once

#include "util/string.h"
#include "gpu_resource.h"

namespace blowbox
{
    /**
    * A buffer type for pixel-based resources like depth buffers 
    * and color buffers. This class is not usable in and of itself,
    * but provides instead a bunch of helper functions for inheriting
    * classes, such as format converters and texture creation.
    *
    * @brief Wrapper for pixel-type resources
    */
	class PixelBuffer : public GpuResource
	{
	public:
        /** @brief Constructs a PixelBuffer resource. */
		PixelBuffer();

        /** @returns The width of the pixel buffer in pixels. */
		const UINT& GetWidth() const { return width_; }

        /** @returns The height of the pixel buffer in pixels. */
		const UINT& GetHeight() const { return height_; }

        /** @returns The resource format of the pixel buffer. */
		const DXGI_FORMAT& GetFormat() const { return format_; }

	protected:
        /**
        * @brief Generates a D3D12_RESOURCE_DESC for Texture2D resources.
        * @param[in] width The width of the Texture2D resource in pixels.
        * @param[in] height The height of the Texture2D resource in pixels
        * @param[in] depth_or_array_size The depth/array size of the resource.
        * @param[in] num_mips The number of mip levels in the resource.
        * @param[in] format The format of the resource.
        * @param[in] flags Any flags that should be set on the resource.
        * @returns A filled out D3D12_RESOURCE_DESC for Texture2D resources.
        */
		D3D12_RESOURCE_DESC DescribeTex2D(UINT width, UINT height, UINT depth_or_array_size, UINT num_mips, DXGI_FORMAT format, UINT flags);

        /**
        * @brief Associates this PixelBuffer with a pre-existing resource (useful for stuff like SwapChain buffers)
        * @param[in] name The name for the resource (any previous names on the resource will be overridden)
        * @param[in] resource The resource to associate this PixelBuffer with.
        * @param[in] current_resource_state The resource state of the resource we're associating with.
        */
		void AssociateWithResource(const WString& name, ID3D12Resource* resource, const D3D12_RESOURCE_STATES& current_resource_state);

        /**
        * @brief Creates a new texture resource for this PixelBuffer.
        * @param[in] name The name for this resource.
        * @param[in] resource_desc The resource description for the resource to be created.
        * @param[in] clear_value The default clear value for this resource.
        */
		void CreateTextureResource(const WString& name, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE& clear_value);

	protected:
        /** @returns A TYPELESS DXGI_FORMAT that can be used inside of D3D12_RESOURCE_DESC's for texture resources. */
		static DXGI_FORMAT GetBaseFormat(DXGI_FORMAT format);

        /** @returns A DXGI_FORMAT compatible with UAV descriptors based on a given DXGI_FORMAT. */
		static DXGI_FORMAT GetUAVFormat(DXGI_FORMAT format);

        /** @returns A DXGI_FORMAT compatible with DSV descriptors based on a given DXGI_FORMAT. */
		static DXGI_FORMAT GetDSVFormat(DXGI_FORMAT format);

        /** @returns A DXGI_FORMAT compatible with SRV descriptors based on a given DXGI_FORMAT. */
		static DXGI_FORMAT GetDepthFormat(DXGI_FORMAT format);

        /** @returns A DXGI_FORMAT for stencil buffers based on a given format. */
		static DXGI_FORMAT GetStencilFormat(DXGI_FORMAT format);

	protected:
		UINT width_;            //!< The width of the pixel buffer in texels
		UINT height_;           //!< The height of the pixel buffer in texels
		UINT array_size_;       //!< The array size of the pixel buffer
		DXGI_FORMAT format_;    //!< The format of the resource itself
	};
}