#include "structured_buffer.h"

#include "renderer/descriptor_heap.h"
#include "renderer/device.h"
#include "core/get.h"
#include "renderer/commands/command_context.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	void StructuredBuffer::Destroy()
	{
		counter_buffer_.Destroy();
		GpuResource::Destroy();
	}
	
	//------------------------------------------------------------------------------------------------------
	void StructuredBuffer::CreateDerivedViews()
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srv_desc.Format = DXGI_FORMAT_UNKNOWN;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv_desc.Buffer.NumElements = element_count_;
		srv_desc.Buffer.StructureByteStride = element_size_;
		srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		srv_id_ = Get::CbvSrvUavHeap()->CreateShaderResourceView(resource_, &srv_desc);

		D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
		uav_desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uav_desc.Format = DXGI_FORMAT_UNKNOWN;
		uav_desc.Buffer.CounterOffsetInBytes = 0;
		uav_desc.Buffer.NumElements = element_count_;
		uav_desc.Buffer.StructureByteStride = element_size_;
		uav_desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		counter_buffer_.Create(L"StructuredBuffer::Counter", 1, 4);

		uav_id_ = Get::CbvSrvUavHeap()->CreateUnorderedAccessView(resource_, counter_buffer_, &uav_desc);
	}

	//------------------------------------------------------------------------------------------------------
	const UINT& StructuredBuffer::GetCounterSRV(CommandContext& context)
	{
		context.TransitionResource(counter_buffer_, D3D12_RESOURCE_STATE_GENERIC_READ);
		return counter_buffer_.GetSRV();
	}

	//------------------------------------------------------------------------------------------------------
	const UINT& StructuredBuffer::GetCounterUAV(CommandContext& context)
	{
		context.TransitionResource(counter_buffer_, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		return counter_buffer_.GetUAV();
	}
}