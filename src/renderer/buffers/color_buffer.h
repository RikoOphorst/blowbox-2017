#pragma once

#include "pixel_buffer.h"

namespace blowbox
{
    /**
    * This buffer types allows you to store color data in a buffer.
    * A ColorBuffer is useful for standard textures such as diffuse,
    * specular, normal, etc maps.
    *
    * @brief Buffer-type for buffers with color data in them.
    */
	class ColorBuffer : public PixelBuffer
	{
	public:
        /** @brief Constructs a ColorBuffer. */
		ColorBuffer();

        /**
        * @brief Constructs a ColorBuffer with an optimized clear value.
        * @param[in] clear_color The clear color that should be used for this buffer when clearing it.
        */
		ColorBuffer(float clear_color[4]);

        /**
        * @brief Creates a ColorBuffer from a SwapChain buffer.
        * @param[in] name The name for the ColorBuffer.
        * @param[in] swap_chain_resource The resource you got from the SwapChain.
        * @param[in] clear_color The clear color that should be used for this buffer when clearing it.
        */
		void CreateFromSwapChain(const WString& name, ID3D12Resource* swap_chain_resource, float clear_color[4]);

        /**
        * @brief Creates a new ColorBuffer.
        * @param[in] name The name for the ColorBuffer.
        * @param[in] width The width of the ColorBuffer in texels.
        * @param[in] height The height of the ColorBuffer in texels.
        * @param[in] format The per-texel data format.
        */
		void Create(const WString& name, UINT width, UINT height, DXGI_FORMAT format);

        /** @returns The Shader Resource View for this ColorBuffer. */
		const UINT& GetSRV() const { return srv_id_; }

        /** @returns The Render Target View for this ColorBuffer. */
		const UINT& GetRTV() const { return rtv_id_; }

        /** @returns The Unordered Access View for this ColorBuffer. */
		const UINT& GetUAV() const { return uav_id_; }

        /** @returns The default clear color for this ColorBuffer. */
		const float* GetClearColor() const { return clear_color_; }

        /** @returns The default clear color for this ColorBuffer. */
		const D3D12_CLEAR_VALUE& GetClearValue() const { return clear_value_; }

	protected:
        /**
        * @brief Creates all the descriptors (views) for this ColorBuffer.
        * @param[in] format The format of the base resource.
        */
		void CreateDerivedViews(DXGI_FORMAT format);

	private:
		UINT srv_id_;                       //!< The SRV for this ColorBuffer.
		UINT rtv_id_;                       //!< The RTV for this ColorBuffer.
		UINT uav_id_;                       //!< The UAV for this ColorBuffer.
		float clear_color_[4];              //!< The clear color for this ColorBuffer.
		D3D12_CLEAR_VALUE clear_value_;     //!< The clear value for this ColorBuffer.
	};
}