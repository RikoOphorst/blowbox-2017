#include "command_queue.h"

#include "renderer/device.h"
#include "renderer/renderer.h"
#include "renderer/commands/command_manager.h"
#include "renderer/commands/command_allocator_pool.h"
#include "core/get.h"

#undef max
#undef min

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type) :
		type_(type)
	{
		ID3D12Device* device = Get::Renderer()->GetDevice()->Get();

		D3D12_COMMAND_QUEUE_DESC queue_desc;
		queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queue_desc.NodeMask = 0;
		queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		queue_desc.Type = type;
		BLOWBOX_ASSERT_HR(device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&queue_)));
		queue_->SetName(L"CommandQueue");

		BLOWBOX_ASSERT_HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)));
		fence_->SetName(L"CommandQueueFence");

		next_fence_value_ = static_cast<uint64_t>(type) << 56 | 1;
		last_completed_fence_value_ = static_cast<uint64_t>(type) << 56;
		fence_event_handle_ = CreateEvent(nullptr, false, false, nullptr);
		assert(fence_event_handle_ != INVALID_HANDLE_VALUE);

		allocator_pool_ = new CommandAllocatorPool(type);
	}
	
	//------------------------------------------------------------------------------------------------------
	CommandQueue::~CommandQueue()
	{
		CloseHandle(fence_event_handle_);

		fence_->Release();
		queue_->Release();
	}

	//------------------------------------------------------------------------------------------------------
	bool CommandQueue::IsFenceComplete(uint64_t fence_value)
	{
		if (fence_value > last_completed_fence_value_)
			last_completed_fence_value_ = eastl::max(last_completed_fence_value_, fence_->GetCompletedValue());

		return fence_value <= last_completed_fence_value_;
	}

	//------------------------------------------------------------------------------------------------------
	uint64_t CommandQueue::IncrementFence()
	{
		queue_->Signal(fence_, next_fence_value_);
		return next_fence_value_++;
	}

	//------------------------------------------------------------------------------------------------------
	void CommandQueue::WaitForFence(uint64_t fence_value)
	{
		if (IsFenceComplete(fence_value))
			return;

		fence_->SetEventOnCompletion(fence_value, fence_event_handle_);
		WaitForSingleObject(fence_event_handle_, INFINITE);
		last_completed_fence_value_ = fence_value;
	}

	//------------------------------------------------------------------------------------------------------
	void CommandQueue::WaitForIdle()
	{
		WaitForFence(next_fence_value_ - 1);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandQueue::StallForFence(uint64_t fence_value)
	{
		CommandQueue* producer = Get::CommandManager()->GetQueue(static_cast<D3D12_COMMAND_LIST_TYPE>(fence_value >> 56));
		queue_->Wait(producer->fence_, fence_value);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandQueue::StallForProducer(CommandQueue* producer)
	{
		assert(producer->next_fence_value_ > 0);
		queue_->Wait(producer->fence_, producer->next_fence_value_ - 1);
	}
	
	//------------------------------------------------------------------------------------------------------
	uint64_t CommandQueue::ExecuteCommandList(ID3D12CommandList* list)
	{
		BLOWBOX_ASSERT_HR(((ID3D12GraphicsCommandList*)list)->Close());
		queue_->ExecuteCommandLists(1, &list);
		queue_->Signal(fence_, next_fence_value_);
		return next_fence_value_++;
	}
	
	//------------------------------------------------------------------------------------------------------
	ID3D12CommandAllocator* CommandQueue::RequestAllocator()
	{
		return allocator_pool_->RequestAllocator(fence_->GetCompletedValue());
	}

	//------------------------------------------------------------------------------------------------------
	void CommandQueue::DiscardAllocator(uint64_t fence_value, ID3D12CommandAllocator* allocator)
	{
		allocator_pool_->DiscardAllocator(fence_value, allocator);
	}
}