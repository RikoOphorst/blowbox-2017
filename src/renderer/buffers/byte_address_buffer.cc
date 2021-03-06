#include "byte_address_buffer.h"

#include "renderer/descriptor_heap.h"
#include "renderer/device.h"
#include "core/get.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	void ByteAddressBuffer::CreateDerivedViews()
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srv_desc.Format = DXGI_FORMAT_R32_TYPELESS;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv_desc.Buffer.NumElements = (UINT)buffer_size_ / 4; // divide by 4 because who the fuck knows ???????
		srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;

		eastl::shared_ptr<DescriptorHeap> cbv_srv_uav_heap = Get::CbvSrvUavHeap();

		srv_id_ = cbv_srv_uav_heap->CreateShaderResourceView(resource_, &srv_desc);

		D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
		uav_desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
		uav_desc.Buffer.NumElements = (UINT)buffer_size_ / 4;
		uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;

		uav_id_ = cbv_srv_uav_heap->CreateUnorderedAccessView(resource_, nullptr, &uav_desc);
	}
}