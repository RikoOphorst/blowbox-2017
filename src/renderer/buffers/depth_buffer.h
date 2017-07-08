#pragma once

#include "pixel_buffer.h"

namespace blowbox
{
	class DepthBuffer : public PixelBuffer
	{
	public:
		DepthBuffer(float depth_clear = 1.0f, UINT stencil_clear = 0);

		void Create(const eastl::wstring& name, UINT width, UINT height, DXGI_FORMAT format);

		const UINT& GetDSV() const					{ return dsv_id_; }
		const UINT& GetDSVDepthReadOnly() const		{ return dsv_depth_read_id_; }
		const UINT& GetDSVStencilReadOnly() const	{ return dsv_stencil_read_id_; }
		const UINT& GetDSVReadOnly() const			{ return dsv_read_id_; }
		const UINT& GetDepthSRV() const				{ return srv_depth_id_; }
		const UINT& GetStencilSRV() const			{ return srv_stencil_id_; }

		const UINT& GetStencilClearValue() const	{ return stencil_clear_; }
		const float& GetDepthClearValue() const		{ return depth_clear_; }

	protected:
		void CreateDerivedViews(DXGI_FORMAT format);

	private:
		UINT dsv_id_;
		UINT dsv_depth_read_id_;
		UINT dsv_stencil_read_id_;
		UINT dsv_read_id_;
		UINT srv_depth_id_;
		UINT srv_stencil_id_;
		UINT stencil_clear_;
		float depth_clear_;
	};
}