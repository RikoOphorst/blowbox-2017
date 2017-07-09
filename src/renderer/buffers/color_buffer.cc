#include "color_buffer.h"

#include "renderer/descriptor_heap.h"
#include "renderer/renderer.h"
#include "renderer/device.h"
#include "core/get.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	ColorBuffer::ColorBuffer()
	{
		clear_color_[0] = 0.0f;
		clear_color_[1] = 0.0f;
		clear_color_[2] = 0.0f;
		clear_color_[3] = 0.0f;
	}
	
	//------------------------------------------------------------------------------------------------------
	ColorBuffer::ColorBuffer(float clear_color[4])
	{
		memcpy(clear_color_, clear_color, sizeof(float) * 4);
	}

	//------------------------------------------------------------------------------------------------------
	void ColorBuffer::CreateFromSwapChain(const eastl::wstring& name, ID3D12Resource* swap_chain_resource)
	{
		AssociateWithResource(name, swap_chain_resource, D3D12_RESOURCE_STATE_PRESENT);

		srv_id_ = BLOWBOX_DESCRIPTOR_ID_UNKNOWN;
		uav_id_ = BLOWBOX_DESCRIPTOR_ID_UNKNOWN;
		rtv_id_ = Get::RtvHeap()->CreateRenderTargetView(resource_, nullptr);
	}

	//------------------------------------------------------------------------------------------------------
	void ColorBuffer::CreateFromTexture(ID3D12Resource* texture_resource, int slice)
	{
		srv_id_ = BLOWBOX_DESCRIPTOR_ID_UNKNOWN;
		uav_id_ = BLOWBOX_DESCRIPTOR_ID_UNKNOWN;

		if (resource_ != nullptr) {
			resource_->Release();
		}

		resource_ = texture_resource;

		D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
		D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};

		D3D12_RESOURCE_DESC text_desc = texture_resource->GetDesc();

		rtv_desc.Format = text_desc.Format;
		rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
		rtv_desc.Texture2DArray.PlaneSlice = 0;
		rtv_desc.Texture2DArray.MipSlice = 0;
		rtv_desc.Texture2DArray.ArraySize = 1;
		rtv_desc.Texture2DArray.FirstArraySlice = D3D12CalcSubresource(0, slice, 0, 1, 1);
		rtv_id_ = Get::RtvHeap()->CreateRenderTargetView(resource_, &rtv_desc);
	}

	//------------------------------------------------------------------------------------------------------
	void ColorBuffer::Create(const eastl::wstring& name, UINT width, UINT height, DXGI_FORMAT format)
	{
		D3D12_RESOURCE_DESC desc = DescribeTex2D(
			width, 
			height, 
			1, // array size, always 1
			1, // num mips, always 1
			format,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS
		);

		clear_value_.Format = format;
		clear_value_.Color[0] = clear_color_[0];
		clear_value_.Color[1] = clear_color_[1];
		clear_value_.Color[2] = clear_color_[2];
		clear_value_.Color[3] = clear_color_[3];

		CreateTextureResource(name, desc, clear_value_);
		CreateDerivedViews(format);
	}

	//------------------------------------------------------------------------------------------------------
	void ColorBuffer::CreateDerivedViews(DXGI_FORMAT format)
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
		D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};

		rtv_desc.Format = format;
		rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtv_desc.Texture2D.MipSlice = 0;
		rtv_desc.Texture2D.PlaneSlice = 0;
		rtv_id_ = Get::RtvHeap()->CreateRenderTargetView(resource_, &rtv_desc);

		uav_desc.Format = GetUAVFormat(format);
		uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		uav_desc.Texture2D.MipSlice = 0;
		uav_desc.Texture2D.PlaneSlice = 0;
		uav_id_ = Get::CbvSrvUavHeap()->CreateUnorderedAccessView(resource_, nullptr, &uav_desc);

		srv_desc.Format = format;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.PlaneSlice = 0;
		srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
		srv_id_ = Get::CbvSrvUavHeap()->CreateShaderResourceView(resource_, &srv_desc);
	}
}