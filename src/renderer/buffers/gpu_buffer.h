#pragma once

#include "util/string.h"

#include "gpu_resource.h"

namespace blowbox
{
	class GpuBuffer : public GpuResource
	{
	public:
		virtual ~GpuBuffer() { Destroy(); }

		virtual void Destroy();

		virtual void Create(const WString& name, UINT num_elements, UINT element_size, void* initial_data = nullptr, bool create_views = true);

		const UINT& GetUAV() const { return uav_id_; }
		const UINT& GetSRV() const { return srv_id_; }
		const D3D12_GPU_VIRTUAL_ADDRESS& GetRootCBV() const { return gpu_virtual_address_; }
		D3D12_GPU_VIRTUAL_ADDRESS GetRootCBV() { return gpu_virtual_address_; }

		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(UINT offset, UINT size, UINT stride) const;
		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(UINT base_vertex_index = 0) const;
		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(UINT offset, UINT size, bool is_32_bit = false) const;
		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(UINT start_index = 0) const;

	protected:
		GpuBuffer();

		D3D12_RESOURCE_DESC DescribeBuffer();
		virtual void CreateDerivedViews() = 0;

		UINT uav_id_;
		UINT srv_id_;
		
		UINT element_count_;
		UINT element_size_;
		UINT buffer_size_;
		D3D12_RESOURCE_FLAGS resource_flags_;
	};
}