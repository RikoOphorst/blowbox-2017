#include "descriptor_heap.h"

#include "util/assert.h"
#include "renderer/device.h"
#include "core/get.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	DescriptorHeap::DescriptorHeap() :
		current_allocations_(0),
		descriptor_size_(0),
		heap_(nullptr)
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	DescriptorHeap::~DescriptorHeap()
	{
		BLOWBOX_RELEASE(heap_);
	}

	//------------------------------------------------------------------------------------------------------
	void DescriptorHeap::Create(const WString& name, D3D12_DESCRIPTOR_HEAP_TYPE heap_type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, UINT descriptor_count)
	{
		heap_desc_.Flags = flags;
		heap_desc_.NodeMask = 0;
		heap_desc_.NumDescriptors = descriptor_count;
		heap_desc_.Type = heap_type;

		BLOWBOX_ASSERT_HR(Get::Device()->Get()->CreateDescriptorHeap(&heap_desc_, IID_PPV_ARGS(&heap_)));

        heap_->SetName(name.c_str());

		descriptor_size_ = Get::Device()->Get()->GetDescriptorHandleIncrementSize(heap_type);
	}

	//------------------------------------------------------------------------------------------------------
	UINT DescriptorHeap::CreateDepthStencilView(ID3D12Resource* dsv_buffer, D3D12_DEPTH_STENCIL_VIEW_DESC* desc)
	{
		assert(current_allocations_ + 1 <= heap_desc_.NumDescriptors);
		assert(heap_desc_.Type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

        Get::Device()->Get()->CreateDepthStencilView(dsv_buffer, desc, GetCPUDescriptorById(current_allocations_));
		current_allocations_ += 1;
		return current_allocations_ - 1;
	}

	//------------------------------------------------------------------------------------------------------
	UINT DescriptorHeap::CreateRenderTargetView(ID3D12Resource* rtv_buffer, D3D12_RENDER_TARGET_VIEW_DESC* desc)
	{
		assert(current_allocations_ + 1 <= heap_desc_.NumDescriptors);
		assert(heap_desc_.Type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        Get::Device()->Get()->CreateRenderTargetView(rtv_buffer, desc, GetCPUDescriptorById(current_allocations_));
		current_allocations_ += 1;
		return current_allocations_ - 1;
	}

	//------------------------------------------------------------------------------------------------------
	UINT DescriptorHeap::CreateConstantBufferView(D3D12_CONSTANT_BUFFER_VIEW_DESC* desc)
	{
		assert(current_allocations_ + 1 <= heap_desc_.NumDescriptors);
		assert(heap_desc_.Type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        Get::Device()->Get()->CreateConstantBufferView(desc, GetCPUDescriptorById(current_allocations_));
		current_allocations_ += 1;
		return current_allocations_ - 1;
	}

	//------------------------------------------------------------------------------------------------------
	UINT DescriptorHeap::CreateShaderResourceView(ID3D12Resource* srv_buffer, D3D12_SHADER_RESOURCE_VIEW_DESC* desc)
	{
		assert(current_allocations_ + 1 <= heap_desc_.NumDescriptors);
		assert(heap_desc_.Type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        Get::Device()->Get()->CreateShaderResourceView(srv_buffer, desc, GetCPUDescriptorById(current_allocations_));
		current_allocations_ += 1;
		return current_allocations_ - 1;
	}

	//------------------------------------------------------------------------------------------------------
	UINT DescriptorHeap::CreateUnorderedAccessView(ID3D12Resource* buffer, ID3D12Resource* counter_buffer, D3D12_UNORDERED_ACCESS_VIEW_DESC* desc)
	{
		assert(current_allocations_ + 1 <= heap_desc_.NumDescriptors);
		assert(heap_desc_.Type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        Get::Device()->Get()->CreateUnorderedAccessView(buffer, counter_buffer, desc, GetCPUDescriptorById(current_allocations_));
		current_allocations_ += 1;
		return current_allocations_ - 1;
	}

	//------------------------------------------------------------------------------------------------------
	UINT DescriptorHeap::CreateSampler(D3D12_SAMPLER_DESC* desc)
	{
		assert(current_allocations_ + 1 <= heap_desc_.NumDescriptors);
		assert(heap_desc_.Type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

        Get::Device()->Get()->CreateSampler(desc, GetCPUDescriptorById(current_allocations_));
		current_allocations_ += 1;
		return current_allocations_ - 1;
	}

	//------------------------------------------------------------------------------------------------------
	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUDescriptorById(UINT id)
	{
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(id, descriptor_size_);

		return handle;
	}
	
	//------------------------------------------------------------------------------------------------------
	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUDescriptorById(UINT id)
	{
		assert(id < current_allocations_);

		auto handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(heap_->GetGPUDescriptorHandleForHeapStart());
		handle.Offset(id, descriptor_size_);

		return handle;
	}

	//------------------------------------------------------------------------------------------------------
	void DescriptorHeap::Clear()
	{
		BLOWBOX_RELEASE(heap_);
		current_allocations_ = 0;
		BLOWBOX_ASSERT_HR(Get::Device()->Get()->CreateDescriptorHeap(&heap_desc_, IID_PPV_ARGS(&heap_)));
	}
}