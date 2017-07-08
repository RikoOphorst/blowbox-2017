#pragma once

#include "pixel_buffer.h"

namespace blowbox
{
	class ColorBuffer : public PixelBuffer
	{
	public:
		ColorBuffer();
		ColorBuffer(float clear_color[4]);

		void CreateFromSwapChain(const eastl::wstring& name, ID3D12Resource* swap_chain_resource);

		void CreateFromTexture(ID3D12Resource* texture_resource, int slice = 0);

		void Create(const eastl::wstring& name, UINT width, UINT height, DXGI_FORMAT format);

		const UINT& GetSRV() const { return srv_id_; }
		const UINT& GetRTV() const { return rtv_id_; }
		const UINT& GetUAV() const { return uav_id_; }
		const float* GetClearColor() const { return clear_color_; }
		const D3D12_CLEAR_VALUE& GetClearValue() const { return clear_value_; }

	protected:
		void CreateDerivedViews(DXGI_FORMAT format);

	private:
		UINT srv_id_;
		UINT rtv_id_;
		UINT uav_id_;
		float clear_color_[4];
		D3D12_CLEAR_VALUE clear_value_;
	};
}