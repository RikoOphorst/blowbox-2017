#pragma once

#include "pixel_buffer.h"

namespace blowbox
{
    /**
    * This buffer wraps all functionality for a depth buffer.
    * DepthBuffers are useful for depth-testing. If you didn't
    * know that, you probably shouldn't be reading this anyway.
    *
    * @brief Wrapper for DepthBuffer-like resources
    */
	class DepthBuffer : public PixelBuffer
	{
	public:
        /**
        * @brief Constructs a DepthBuffer with a preset depth and stencil clear value.
        * @param[in] depth_clear The default value the depth part of the buffer should be cleared to.
        * @param[in] stencil_clear The default value the stencil part of the buffer should be cleared to.
        */
		DepthBuffer(float depth_clear = 1.0f, UINT stencil_clear = 0);

        /**
        * @brief Creates the actual DepthBuffer.
        * @param[in] name The name of the DepthBuffer resource.
        * @param[in] width The width of the DepthBuffer resource in texels.
        * @param[in] height The height of the DepthBuffer resource in texels.
        * @param[in] format The format of the DepthBuffer. From this, it is automatically determined if it should have a stencil part or not.
        */
		void Create(const eastl::wstring& name, UINT width, UINT height, DXGI_FORMAT format);

        /** @returns The Depth Stencil View for this DepthBuffer. */
		const UINT& GetDSV() const					{ return dsv_id_; }

        /** @returns A depth-read-only Depth Stencil View for this DepthBuffer. */
		const UINT& GetDSVDepthReadOnly() const		{ return dsv_depth_read_id_; }

        /** @returns A stencil-read-only Depth Stencil View for this DepthBuffer. */
		const UINT& GetDSVStencilReadOnly() const	{ return dsv_stencil_read_id_; }

        /** @returns A read-only Depth Stencil View for this DepthBuffer. */
		const UINT& GetDSVReadOnly() const			{ return dsv_read_id_; }

        /** @returns A Shader Resource View for the depth part of this DepthBuffer. */
		const UINT& GetDepthSRV() const				{ return srv_depth_id_; }

        /** @returns A Shader Resource View for the stencil part of this DepthBuffer. */
		const UINT& GetStencilSRV() const			{ return srv_stencil_id_; }

        /** @returns The stencil clear value for this DepthBuffer. */
		const UINT& GetStencilClearValue() const	{ return stencil_clear_; }

        /** @returns The depth clear value for this DepthBuffer. */
		const float& GetDepthClearValue() const		{ return depth_clear_; }

	protected:
        /** @brief Creates all the resource descriptors (views) for this resource. */
		void CreateDerivedViews(DXGI_FORMAT format);

	private:
		UINT dsv_id_;               //!< The base Depth Stencil View for this DepthBuffer.
		UINT dsv_depth_read_id_;    //!< The Depth Stencil View for this DepthBuffer with depth read only.
		UINT dsv_stencil_read_id_;  //!< The Depth Stencil View for this DepthBuffer with stencil read only.
		UINT dsv_read_id_;          //!< The Depth Stencil View for this DepthBuffer which is completely read only.
		UINT srv_depth_id_;         //!< The Shader Resource View for this DepthBuffer for the depth part.
		UINT srv_stencil_id_;       //!< The Shader Resource View for this DepthBuffer for the stencil part.
		UINT stencil_clear_;        //!< The stencil clear value.
		float depth_clear_;         //!< The depth clear value.
	};
}