#pragma once

#include "core/eastl_required.h"
#include <EASTL/string.h>
#include "gpu_resource.h"

namespace blowbox
{
	class PixelBuffer : public GpuResource
	{
	public:
		PixelBuffer();

		const UINT& GetWidth() const { return width_; }
		const UINT& GetHeight() const { return height_; }
		const DXGI_FORMAT& GetFormat() const { return format_; }

	protected:
		D3D12_RESOURCE_DESC DescribeTex2D(UINT width, UINT height, UINT depth_or_array_size, UINT num_mips, DXGI_FORMAT format, UINT flags);

		void AssociateWithResource(const eastl::wstring& name, ID3D12Resource* resource, const D3D12_RESOURCE_STATES& current_resource_state);
		void CreateTextureResource(const eastl::wstring& name, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE& clear_value);

	protected:

		static DXGI_FORMAT GetBaseFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetUAVFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetDSVFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetDepthFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetStencilFormat(DXGI_FORMAT format);

	protected:
		UINT width_;
		UINT height_;
		UINT array_size_;
		DXGI_FORMAT format_;
	};
}