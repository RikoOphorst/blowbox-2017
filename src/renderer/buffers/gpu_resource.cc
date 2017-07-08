#include "gpu_resource.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	GpuResource::GpuResource() :
		resource_(nullptr),
		usage_state_(D3D12_RESOURCE_STATE_COMMON),
		transition_state_(D3D12_RESOURCE_STATE_INVALID),
		gpu_virtual_address_(D3D12_GPU_VIRTUAL_ADDRESS_NULL)
	{

	}
	
	//------------------------------------------------------------------------------------------------------
	GpuResource::GpuResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES current_state) :
		resource_(resource),
		usage_state_(current_state),
		transition_state_(D3D12_RESOURCE_STATE_INVALID)
	{
		gpu_virtual_address_ = resource->GetGPUVirtualAddress();
	}
	
	//------------------------------------------------------------------------------------------------------
	GpuResource::~GpuResource()
	{
		Destroy();
	}

	//------------------------------------------------------------------------------------------------------
	void GpuResource::Associate(ID3D12Resource* resource, D3D12_RESOURCE_STATES current_state)
	{
		resource_ = resource;
		usage_state_ = current_state;
		transition_state_ = D3D12_RESOURCE_STATE_INVALID;
		gpu_virtual_address_ = resource->GetGPUVirtualAddress();
	}
	
	//------------------------------------------------------------------------------------------------------
	void GpuResource::Destroy()
	{
		SAFE_RELEASE(resource_);
	}
}