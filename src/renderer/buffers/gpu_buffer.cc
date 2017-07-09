#include "gpu_buffer.h"

#include "core/get.h"
#include "renderer/device.h"
#include "renderer/descriptor_heap.h"
#include "renderer/commands/command_context.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	void GpuBuffer::Destroy()
	{
	}
	
	//------------------------------------------------------------------------------------------------------
	void GpuBuffer::Create(const eastl::wstring& name, UINT num_elements, UINT element_size, void* initial_data, bool create_views)
	{
		element_count_ = num_elements;
		element_size_ = element_size;
		buffer_size_ = num_elements * element_size;
		usage_state_ = D3D12_RESOURCE_STATE_COMMON;

		D3D12_RESOURCE_DESC resource_desc = DescribeBuffer();

		D3D12_HEAP_PROPERTIES heap_properties; 
		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_properties.CreationNodeMask = 1;
		heap_properties.VisibleNodeMask = 1;

		BLOWBOX_ASSERT_HR(Get::Device()->Get()->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc, usage_state_, nullptr, IID_PPV_ARGS(&resource_)));

		gpu_virtual_address_ = resource_->GetGPUVirtualAddress();

		if (initial_data != nullptr)
		{
			CommandContext::InitializeBuffer(*this, initial_data, buffer_size_);
		}

		resource_->SetName(name.c_str());

		if (create_views)
		{
			CreateDerivedViews();
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	D3D12_VERTEX_BUFFER_VIEW GpuBuffer::GetVertexBufferView(UINT offset, UINT size, UINT stride) const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = gpu_virtual_address_ + static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(offset);
		vbv.SizeInBytes = size;
		vbv.StrideInBytes = stride;
		return vbv;
	}

	//------------------------------------------------------------------------------------------------------
	D3D12_VERTEX_BUFFER_VIEW GpuBuffer::GetVertexBufferView(UINT base_vertex_index) const
	{
		UINT offset = base_vertex_index * element_size_;
		return GetVertexBufferView(offset, buffer_size_ - offset, element_size_);
	}

	//------------------------------------------------------------------------------------------------------
	D3D12_INDEX_BUFFER_VIEW GpuBuffer::GetIndexBufferView(UINT offset, UINT size, bool is_32_bit) const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = gpu_virtual_address_ + offset;
		ibv.Format = is_32_bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
		ibv.SizeInBytes = size;
		return ibv;
	}

	//------------------------------------------------------------------------------------------------------
	D3D12_INDEX_BUFFER_VIEW GpuBuffer::GetIndexBufferView(UINT start_index) const
	{
		size_t offset = start_index * element_size_;
		return GetIndexBufferView(static_cast<UINT>(offset), static_cast<UINT>(buffer_size_ - offset), element_size_ == 4);
	}

	//------------------------------------------------------------------------------------------------------
	GpuBuffer::GpuBuffer() : buffer_size_(0), element_count_(0), element_size_(0)
	{
		resource_flags_ = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		uav_id_ = BLOWBOX_DESCRIPTOR_ID_UNKNOWN;
		srv_id_ = BLOWBOX_DESCRIPTOR_ID_UNKNOWN;
	}
	
	//------------------------------------------------------------------------------------------------------
	D3D12_RESOURCE_DESC GpuBuffer::DescribeBuffer()
	{
		assert(buffer_size_ != 0);

		D3D12_RESOURCE_DESC desc = {};
		desc.Alignment = 0;
		desc.DepthOrArraySize = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Flags = resource_flags_;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Height = 1;
		desc.Width = (UINT64)buffer_size_;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		return desc;
	}
}