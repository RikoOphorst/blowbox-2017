#include "depth_buffer.h"

#include "renderer/descriptor_heap.h"
#include "renderer/device.h"
#include "core/get.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	DepthBuffer::DepthBuffer(float depth_clear, UINT stencil_clear) :
		depth_clear_(depth_clear),
		stencil_clear_(stencil_clear),
		dsv_id_(BLOWBOX_DESCRIPTOR_ID_UNKNOWN),
		dsv_depth_read_id_(BLOWBOX_DESCRIPTOR_ID_UNKNOWN),
		dsv_stencil_read_id_(BLOWBOX_DESCRIPTOR_ID_UNKNOWN),
		dsv_read_id_(BLOWBOX_DESCRIPTOR_ID_UNKNOWN),
		srv_depth_id_(BLOWBOX_DESCRIPTOR_ID_UNKNOWN),
		srv_stencil_id_(BLOWBOX_DESCRIPTOR_ID_UNKNOWN)
	{
		
	}
	
	//------------------------------------------------------------------------------------------------------
	void DepthBuffer::Create(const eastl::wstring& name, UINT width, UINT height, DXGI_FORMAT format)
	{
		D3D12_RESOURCE_DESC desc = DescribeTex2D(width, height, 1, 1, format, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		D3D12_CLEAR_VALUE clear_value = {};
		clear_value.Format = format;
		clear_value.Color[0] = depth_clear_;
		clear_value.Color[1] = depth_clear_;
		clear_value.Color[2] = depth_clear_;
		clear_value.Color[3] = depth_clear_;

		CreateTextureResource(name, desc, clear_value);
		CreateDerivedViews(format);
	}
	
	//------------------------------------------------------------------------------------------------------
	void DepthBuffer::CreateDerivedViews(DXGI_FORMAT format)
	{
		SharedPtr<DescriptorHeap> dsv_heap = Get::DsvHeap();
		SharedPtr<DescriptorHeap> srv_heap = Get::CbvSrvUavHeap();

		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		dsv_desc.Format = GetDSVFormat(format);
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Texture2D.MipSlice = 0;

		dsv_desc.Flags = D3D12_DSV_FLAG_NONE;
		dsv_id_ = dsv_heap->CreateDepthStencilView(resource_, &dsv_desc);

		dsv_desc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
		dsv_depth_read_id_ = dsv_heap->CreateDepthStencilView(resource_, &dsv_desc);

		DXGI_FORMAT stencil_format = GetStencilFormat(format);

		if (stencil_format != DXGI_FORMAT_UNKNOWN)
		{
			dsv_desc.Flags = D3D12_DSV_FLAG_READ_ONLY_STENCIL;
			dsv_stencil_read_id_ = dsv_heap->CreateDepthStencilView(resource_, &dsv_desc);

			dsv_desc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH | D3D12_DSV_FLAG_READ_ONLY_STENCIL;
			dsv_read_id_ = dsv_heap->CreateDepthStencilView(resource_, &dsv_desc);
		}
		else
		{
			dsv_stencil_read_id_ = dsv_id_;
			dsv_read_id_ = dsv_depth_read_id_;
		}

		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format = GetDepthFormat(format);
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		srv_depth_id_ = srv_heap->CreateShaderResourceView(resource_, &srv_desc);

		if (stencil_format != DXGI_FORMAT_UNKNOWN)
		{
			srv_desc.Format = stencil_format;
			srv_stencil_id_ = srv_heap->CreateShaderResourceView(resource_, &srv_desc);
		}
	}
}