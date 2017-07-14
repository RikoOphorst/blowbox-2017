#include "command_context.h"

#include "core/get.h"
#include "renderer/device.h"
#include "renderer/commands/command_queue.h"
#include "renderer/commands/command_manager.h"
#include "renderer/commands/command_context_manager.h"
#include "renderer/commands/compute_context.h"
#include "renderer/commands/graphics_context.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE type) :
		list_(nullptr),
		allocator_(nullptr),
		type_(type),
		num_flushable_barriers_(0),
		compute_pipeline_state_(nullptr),
		compute_root_signature_(nullptr),
		graphics_pipeline_state_(nullptr),
		graphics_root_signature_(nullptr)
	{
		ZeroMemory(&descriptor_heaps_, sizeof(ID3D12DescriptorHeap*) * 4);
	}
	
	//------------------------------------------------------------------------------------------------------
	CommandContext::~CommandContext()
	{
		BLOWBOX_RELEASE(list_);
	}

	//------------------------------------------------------------------------------------------------------
	CommandContext& CommandContext::Begin(const WString& name)
	{
		CommandContext* context = Get::CommandContextManager()->RequestContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
		context->SetName(name);
		return *context;
	}

	//------------------------------------------------------------------------------------------------------
	GraphicsContext& CommandContext::GetGraphicsContext()
	{
		assert(type_ != D3D12_COMMAND_LIST_TYPE_COMPUTE); // Converting to a compute context is invalid
		return reinterpret_cast<GraphicsContext&>(*this);
	}

	//------------------------------------------------------------------------------------------------------
	ComputeContext& CommandContext::GetComputeContext()
	{
		return reinterpret_cast<ComputeContext&>(*this);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::Initialize()
	{
		Get::CommandManager()->CreateCommandList(type_, &list_, &allocator_);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::InitializeTexture(GpuResource& dest_resource, UINT num_subresources, D3D12_SUBRESOURCE_DATA subresource_data[])
	{
		ID3D12Resource* upload_buffer = nullptr;

		CommandContext& context = CommandContext::Begin();

		eastl::shared_ptr<Device> device = Get::Device();

		UINT64 texture_upload_buffer_size;
		device->Get()->GetCopyableFootprints(&dest_resource->GetDesc(), 0, 1, 0, nullptr, nullptr, nullptr, &texture_upload_buffer_size);

		BLOWBOX_ASSERT_HR(
			device->Get()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(texture_upload_buffer_size),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&upload_buffer)
			)
		);

        context.TransitionResource(dest_resource, D3D12_RESOURCE_STATE_COPY_DEST, true);
		// copy data to the intermediate upload heap and then schedule a copy from the upload heap to the default texture
		UpdateSubresources(context.list_, dest_resource.Get(), upload_buffer, 0, 0, num_subresources, subresource_data);
		context.TransitionResource(dest_resource, D3D12_RESOURCE_STATE_GENERIC_READ, true);

		// Execute the command list and wait for it to finish so we can release the upload buffer
		context.Finish(true);
		
		upload_buffer->Release();
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::InitializeBuffer(GpuResource& dest_resource, const void* data, UINT num_bytes, bool use_offset, UINT offset)
	{
		ID3D12Resource* upload_buffer;

		CommandContext& context = CommandContext::Begin();

		D3D12_HEAP_PROPERTIES heap_properties;
		heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_properties.CreationNodeMask = 1;
		heap_properties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC buffer_desc;
		buffer_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		buffer_desc.Alignment = 0;
		buffer_desc.Width = dest_resource->GetDesc().Width;
		buffer_desc.Height = 1;
		buffer_desc.DepthOrArraySize = 1;
		buffer_desc.MipLevels = 1;
		buffer_desc.Format = DXGI_FORMAT_UNKNOWN;
		buffer_desc.SampleDesc.Count = 1;
		buffer_desc.SampleDesc.Quality = 0;
		buffer_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		buffer_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		BLOWBOX_ASSERT_HR(
			Get::Device()->Get()->CreateCommittedResource(
				&heap_properties, 
				D3D12_HEAP_FLAG_NONE, 
				&buffer_desc, 
				D3D12_RESOURCE_STATE_GENERIC_READ, 
				nullptr, 
				IID_PPV_ARGS(&upload_buffer)
			)
		);

		// map, copy, unmap...
		void* dest_address;
		upload_buffer->Map(0, nullptr, &dest_address);
		memcpy(dest_address, data, num_bytes);
		upload_buffer->Unmap(0, nullptr);

		// copy data to the intermediate upload heap and then schedule a copy from the upload heap to the default buffer
		context.TransitionResource(dest_resource, D3D12_RESOURCE_STATE_COPY_DEST, true);
		if (use_offset)
		{
			context.list_->CopyBufferRegion(dest_resource, offset, upload_buffer, 0, num_bytes);
		}
		else
		{
			context.list_->CopyResource(dest_resource, upload_buffer);
		}

		context.TransitionResource(dest_resource, D3D12_RESOURCE_STATE_GENERIC_READ, true);

		// Execute the command list and wait for it to finish so we can release the upload buffer
		context.Finish(true);

		upload_buffer->Release();
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::CopyBuffer(GpuResource& dest_resource, UploadBuffer& source_resource)
	{
		TransitionResource(dest_resource, D3D12_RESOURCE_STATE_COPY_DEST);
		FlushResourceBarriers();
		list_->CopyResource(dest_resource, source_resource);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::CopyBuffer(GpuResource& dest_resource, GpuResource& source_resource)
	{
		TransitionResource(dest_resource, D3D12_RESOURCE_STATE_COPY_DEST);
		TransitionResource(source_resource, D3D12_RESOURCE_STATE_COPY_SOURCE);
		FlushResourceBarriers();
		list_->CopyResource(dest_resource, source_resource);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::CopyBufferRegion(GpuResource& dest_resource, UINT dest_offset, GpuResource& source_resource, UINT source_offset, UINT num_bytes)
	{
		TransitionResource(dest_resource, D3D12_RESOURCE_STATE_COPY_DEST);
		TransitionResource(source_resource, D3D12_RESOURCE_STATE_COPY_SOURCE);
		FlushResourceBarriers();
		list_->CopyBufferRegion(dest_resource, dest_offset, source_resource, source_offset, num_bytes);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::CopySubresource(GpuResource& dest_resource, UINT dest_resource_index, GpuResource& source_resource, UINT source_sub_index)
	{
		FlushResourceBarriers();

		D3D12_TEXTURE_COPY_LOCATION dest_location =
		{
			dest_resource.Get(),
			D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
			dest_resource_index
		};

		D3D12_TEXTURE_COPY_LOCATION source_location =
		{
			source_resource.Get(),
			D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
			source_sub_index
		};

		list_->CopyTextureRegion(&dest_location, 0, 0, 0, &source_location, nullptr);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::CopyCounter(GpuResource& dest_resource, UINT dest_offset, StructuredBuffer& source_resource)
	{
		TransitionResource(dest_resource, D3D12_RESOURCE_STATE_COPY_DEST);
		TransitionResource(source_resource.GetCounterBuffer(), D3D12_RESOURCE_STATE_COPY_SOURCE);
		FlushResourceBarriers();
		list_->CopyBufferRegion(dest_resource, dest_offset, source_resource.GetCounterBuffer(), 0, 4);
	}
	
	//------------------------------------------------------------------------------------------------------
	uint64_t CommandContext::Flush(bool wait_for_completion)
	{
		eastl::shared_ptr<CommandManager> command_manager = Get::CommandManager();

		uint64_t fence_value = command_manager->GetQueue(type_)->ExecuteCommandList(list_);
		
		if (wait_for_completion)
		{
			command_manager->WaitForFence(fence_value);
		}

		list_->Reset(allocator_, nullptr);

		return fence_value;
	}
	
	//------------------------------------------------------------------------------------------------------
	uint64_t CommandContext::Finish(bool wait_for_completion)
	{
		BLOWBOX_ASSERT(type_ == D3D12_COMMAND_LIST_TYPE_DIRECT || type_ == D3D12_COMMAND_LIST_TYPE_COMPUTE);

		FlushResourceBarriers();

		eastl::shared_ptr<CommandContextManager> command_context_manager = Get::CommandContextManager();
		eastl::shared_ptr<CommandManager> command_manager = Get::CommandManager();

		auto queue = command_manager->GetQueue(type_);

		uint64_t fence_value = queue->ExecuteCommandList(list_);

		queue->DiscardAllocator(fence_value, allocator_);
		allocator_ = nullptr;

		if (wait_for_completion)
		{
			command_manager->WaitForFence(fence_value);
		}

		command_context_manager->DiscardContext(this);

		return fence_value;
	}

    //------------------------------------------------------------------------------------------------------
    void CommandContext::TransitionResource(GpuResource& resource, const D3D12_RESOURCE_STATES& new_state, bool flush_immediate)
	{
		auto& old_state = resource.GetState();

		if (old_state != new_state)
		{
			assert(num_flushable_barriers_ < COMMAND_CONTEXT_BARRIER_BUFFER_SIZE); // Cannot exceed resource barrier buffer size
			D3D12_RESOURCE_BARRIER& barrier = resource_barrier_buffer_[num_flushable_barriers_++];

			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = resource;
			barrier.Transition.StateAfter = new_state;
			barrier.Transition.StateBefore = old_state;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			// check if the resource transition was already started using CommandContext::BeginResourceTransition()
			if (new_state == resource.transition_state_)
			{
				// only finish the transition (FLAG_END_ONLY) & set transition state to invalid
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
				resource.transition_state_ = D3D12_RESOURCE_STATE_INVALID;
			}
			else
			{
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			}

			resource.usage_state_ = new_state;
		}

		if (flush_immediate == true || num_flushable_barriers_ >= 16)
		{
			FlushResourceBarriers();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::BeginResourceTransition(GpuResource& resource, const D3D12_RESOURCE_STATES& new_state, bool flush_immediate)
	{
		// is the resource already transitioning? then finish it
		if (resource.transition_state_ != D3D12_RESOURCE_STATE_INVALID)
			TransitionResource(resource, new_state, false);

		D3D12_RESOURCE_STATES& old_state = resource.usage_state_;

		if (old_state != new_state)
		{
			assert(num_flushable_barriers_ < COMMAND_CONTEXT_BARRIER_BUFFER_SIZE); // Cannot exceed resource barrier buffer size
			D3D12_RESOURCE_BARRIER& barrier = resource_barrier_buffer_[num_flushable_barriers_++];

			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = resource;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = old_state;
			barrier.Transition.StateAfter = new_state;

			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY;

			resource.transition_state_ = new_state;
		}
		else if (new_state == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
		{
			InsertUAVBarrier(resource, flush_immediate);
		}

		if (flush_immediate == true || num_flushable_barriers_ >= 16)
		{
			FlushResourceBarriers();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::InsertUAVBarrier(GpuResource& resource, bool flush_immediate)
	{
		assert(num_flushable_barriers_ < COMMAND_CONTEXT_BARRIER_BUFFER_SIZE); // Cannot exceed resource barrier buffer size
		D3D12_RESOURCE_BARRIER& barrier = resource_barrier_buffer_[num_flushable_barriers_++];

		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.UAV.pResource = resource;

		if (flush_immediate == true || num_flushable_barriers_ >= 16)
		{
			FlushResourceBarriers();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::InsertAliasBuffer(GpuResource& before, GpuResource& after, bool flush_immediate)
	{
		assert(num_flushable_barriers_ < COMMAND_CONTEXT_BARRIER_BUFFER_SIZE); // Cannot exceed resource barrier buffer size
		D3D12_RESOURCE_BARRIER& barrier = resource_barrier_buffer_[num_flushable_barriers_++];

		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Aliasing.pResourceBefore = before;
		barrier.Aliasing.pResourceAfter = after;

		if (flush_immediate == true || num_flushable_barriers_ >= 16)
		{
			FlushResourceBarriers();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::FlushResourceBarriers()
	{
		if (num_flushable_barriers_ > 0)
		{
			list_->ResourceBarrier(num_flushable_barriers_, resource_barrier_buffer_);
			num_flushable_barriers_ = 0;
		}
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::InsertTimeStamp(ID3D12QueryHeap* query_heap, UINT query_idx)
	{
		list_->EndQuery(query_heap, D3D12_QUERY_TYPE_TIMESTAMP, query_idx);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::ResolveTimeStamps(ID3D12Resource* read_back_heap, ID3D12QueryHeap* query_heap, UINT num_queries)
	{
		list_->ResolveQueryData(query_heap, D3D12_QUERY_TYPE_TIMESTAMP, 0, num_queries, read_back_heap, 0);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12DescriptorHeap* heap)
	{
		if (descriptor_heaps_[type] != heap)
		{
			descriptor_heaps_[type] = heap;
			BindDescriptorHeaps();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::SetDescriptorHeaps(UINT heap_count, D3D12_DESCRIPTOR_HEAP_TYPE heap_types[], ID3D12DescriptorHeap* heaps[])
	{
		bool updated = false;

		for (UINT i = 0; i < heap_count; i++)
		{
			if (descriptor_heaps_[heap_types[i]] != heaps[i])
			{
				descriptor_heaps_[heap_types[i]] = heaps[i];
				updated = true;
			}
		}

		if (updated)
		{
			BindDescriptorHeaps();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::SetPredication(ID3D12Resource* buffer, UINT64 buffer_offset, D3D12_PREDICATION_OP op)
	{
		list_->SetPredication(buffer, buffer_offset, op);
	}
	
	//------------------------------------------------------------------------------------------------------
	ID3D12GraphicsCommandList* CommandContext::Get()
	{
		return list_;
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContext::BindDescriptorHeaps()
	{
		UINT num_heaps = 0;

		ID3D12DescriptorHeap* heaps_to_bind[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

		for (UINT i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; i++)
		{
			if (descriptor_heaps_[i] != nullptr)
			{
				heaps_to_bind[num_heaps++] = descriptor_heaps_[i];
			}
		}

		if (num_heaps > 0)
		{
			list_->SetDescriptorHeaps(num_heaps, heaps_to_bind);
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	void CommandContext::Reset()
	{
		BLOWBOX_ASSERT(list_ != nullptr && allocator_ == nullptr);
		allocator_ = Get::CommandManager()->GetQueue(type_)->RequestAllocator();
		list_->Reset(allocator_, nullptr);

		graphics_pipeline_state_ = nullptr;
		graphics_root_signature_ = nullptr;
		compute_pipeline_state_ = nullptr;
		compute_root_signature_ = nullptr;
		num_flushable_barriers_ = 0;

		BindDescriptorHeaps();
	}
}