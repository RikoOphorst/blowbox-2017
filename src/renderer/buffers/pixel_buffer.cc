#include "pixel_buffer.h"

#include "renderer/device.h"
#include "core/get.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	PixelBuffer::PixelBuffer() :
		width_(0),
		height_(0),
		array_size_(0),
		format_(DXGI_FORMAT_UNKNOWN)
	{
	}

	//------------------------------------------------------------------------------------------------------
	D3D12_RESOURCE_DESC PixelBuffer::DescribeTex2D(UINT width, UINT height, UINT depth_or_array_size, UINT num_mips, DXGI_FORMAT format, UINT flags)
	{
		width_ = width;
		height_ = height;
		array_size_ = depth_or_array_size;
		format_ = format;

		D3D12_RESOURCE_DESC desc = {};
		desc.Alignment = 0;
		desc.DepthOrArraySize = static_cast<UINT16>(depth_or_array_size);
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(flags);
		desc.Format = GetBaseFormat(format);
		desc.Height = static_cast<UINT>(height);
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.MipLevels = num_mips;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Width = static_cast<UINT64>(width);

		return desc;
	}

	//------------------------------------------------------------------------------------------------------
	void PixelBuffer::AssociateWithResource(const WString& name, ID3D12Resource* resource, const D3D12_RESOURCE_STATES& current_resource_state)
	{
		assert(resource != nullptr);
		if (resource_ != nullptr)
		{
			resource_->Release();
		}
		resource_ = resource;

		D3D12_RESOURCE_DESC desc = resource->GetDesc();

		width_ = static_cast<UINT>(desc.Width); // TODO: this doesnt support large virtual textures, needs fix
		height_ = desc.Height;
		array_size_ = desc.DepthOrArraySize;
		format_ = desc.Format;

		resource_->SetName(name.c_str());

        usage_state_ = current_resource_state;
	}

	//------------------------------------------------------------------------------------------------------
	void PixelBuffer::CreateTextureResource(const WString& name, const D3D12_RESOURCE_DESC& resource_desc, const D3D12_CLEAR_VALUE& clear_value)
	{
		BLOWBOX_ASSERT_HR(
			Get::Device()->Get()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), 
				D3D12_HEAP_FLAG_NONE, 
				&resource_desc, 
				D3D12_RESOURCE_STATE_COMMON, 
				&clear_value, 
				IID_PPV_ARGS(&resource_)
			)
		);

		usage_state_ = D3D12_RESOURCE_STATE_COMMON;
		gpu_virtual_address_ = D3D12_GPU_VIRTUAL_ADDRESS_NULL; // texture resources cannot be queried for their gpu virtual address

		resource_->SetName(name.c_str());
	}

	//------------------------------------------------------------------------------------------------------
	DXGI_FORMAT PixelBuffer::GetBaseFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return DXGI_FORMAT_R8G8B8A8_TYPELESS;

		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			return DXGI_FORMAT_B8G8R8A8_TYPELESS;

		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return DXGI_FORMAT_B8G8R8X8_TYPELESS;

			// 32-bit Z w/ Stencil
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return DXGI_FORMAT_R32G8X24_TYPELESS;

			// No Stencil
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
			return DXGI_FORMAT_R32_TYPELESS;

			// 24-bit Z
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			return DXGI_FORMAT_R24G8_TYPELESS;

			// 16-bit Z w/o Stencil
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
			return DXGI_FORMAT_R16_TYPELESS;

		default:
			return format;
		}
	}

	//------------------------------------------------------------------------------------------------------
	DXGI_FORMAT PixelBuffer::GetUAVFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			return DXGI_FORMAT_B8G8R8A8_UNORM;

		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return DXGI_FORMAT_B8G8R8X8_UNORM;

		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_R32_FLOAT:
			return DXGI_FORMAT_R32_FLOAT;

#ifdef _DEBUG
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_D16_UNORM:

			BLOWBOX_ASSERT(false && "Requested a UAV format for a depth stencil format.");
            break;
#endif

		default:
			return format;
		}
	}

	//------------------------------------------------------------------------------------------------------
	DXGI_FORMAT PixelBuffer::GetDSVFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
			// 32-bit Z w/ Stencil
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

			// No Stencil
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
			return DXGI_FORMAT_D32_FLOAT;

			// 24-bit Z
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;

			// 16-bit Z w/o Stencil
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
			return DXGI_FORMAT_D16_UNORM;

		default:
			return format;
		}
	}

	//------------------------------------------------------------------------------------------------------
	DXGI_FORMAT PixelBuffer::GetDepthFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
			// 32-bit Z w/ Stencil
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;

			// No Stencil
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
			return DXGI_FORMAT_R32_FLOAT;

			// 24-bit Z
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

			// 16-bit Z w/o Stencil
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
			return DXGI_FORMAT_R16_UNORM;

		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	DXGI_FORMAT PixelBuffer::GetStencilFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
			// 32-bit Z w/ Stencil
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

			// 24-bit Z
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			return DXGI_FORMAT_X24_TYPELESS_G8_UINT;

		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}
}