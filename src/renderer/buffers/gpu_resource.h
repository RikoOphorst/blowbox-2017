#pragma once

#include "renderer/d3d12_includes.h"

#define D3D12_RESOURCE_STATE_INVALID ((D3D12_RESOURCE_STATES)-1)

namespace blowbox
{
	class GpuResource
	{
		friend class CommandContext;
	public:
		GpuResource();
		GpuResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES current_state);
		virtual ~GpuResource();

		void Associate(ID3D12Resource* resource, D3D12_RESOURCE_STATES current_state);

		void Destroy();

		ID3D12Resource* operator->() { return resource_; }
		const ID3D12Resource* operator->() const { return resource_; }

		operator ID3D12Resource*() { return resource_; }

		ID3D12Resource* Get() { return resource_; }

		const D3D12_RESOURCE_STATES& GetState() const { return usage_state_; }
		const D3D12_RESOURCE_STATES& GetTransitionState() const { return transition_state_; }

	protected:
		ID3D12Resource* resource_;
		D3D12_RESOURCE_STATES usage_state_;
		D3D12_RESOURCE_STATES transition_state_;
		D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address_;
	};
}