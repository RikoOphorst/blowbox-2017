#include "command_manager.h"

#include "renderer/device.h"
#include "renderer/commands/command_queue.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	CommandManager::CommandManager() :
		device_(nullptr),
        graphics_queue_(nullptr),
        compute_queue_(nullptr),
        copy_queue_(nullptr)
	{
		
	}
	
	//------------------------------------------------------------------------------------------------------
	CommandManager::~CommandManager()
	{
		delete graphics_queue_;
		delete compute_queue_;
		delete copy_queue_;
	}

	//------------------------------------------------------------------------------------------------------
	void CommandManager::Startup(Device* device)
	{
        BLOWBOX_ASSERT(device_ == nullptr);
        BLOWBOX_ASSERT(graphics_queue_ == nullptr);
        BLOWBOX_ASSERT(compute_queue_ == nullptr);
        BLOWBOX_ASSERT(copy_queue_ == nullptr);

		device_ = device;
		graphics_queue_ = new CommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
		compute_queue_ = new CommandQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE);
		copy_queue_ = new CommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
	}

	//------------------------------------------------------------------------------------------------------
	void CommandManager::CreateCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** out_list, ID3D12CommandAllocator** out_allocator)
	{
		BLOWBOX_ASSERT(type != D3D12_COMMAND_LIST_TYPE_BUNDLE);

		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			*out_allocator = graphics_queue_->RequestAllocator();
			break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
			*out_allocator = compute_queue_->RequestAllocator();
			break;
		case D3D12_COMMAND_LIST_TYPE_COPY:
			*out_allocator = copy_queue_->RequestAllocator();
			break;
		case D3D12_COMMAND_LIST_TYPE_BUNDLE:
			break;
		}

		device_->Get()->CreateCommandList(0, type, *out_allocator, nullptr, IID_PPV_ARGS(out_list));
		(*out_list)->SetName(L"CommandList");
	}

	//------------------------------------------------------------------------------------------------------
	CommandQueue* CommandManager::GetQueue(D3D12_COMMAND_LIST_TYPE type)
	{
		assert(type != D3D12_COMMAND_LIST_TYPE_BUNDLE);

		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			return graphics_queue_;
			break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
			return compute_queue_;
			break;
		case D3D12_COMMAND_LIST_TYPE_COPY:
			return copy_queue_;
			break;
		default:
			return graphics_queue_;
		}
	}

	//------------------------------------------------------------------------------------------------------
	CommandQueue* CommandManager::GetCommandQueue()
	{
		return graphics_queue_;
	}

	//------------------------------------------------------------------------------------------------------
	CommandQueue* CommandManager::GetGraphicsQueue()
	{
		return graphics_queue_;
	}

	//------------------------------------------------------------------------------------------------------
	CommandQueue* CommandManager::GetComputeQueue()
	{
		return compute_queue_;
	}

	//------------------------------------------------------------------------------------------------------
	CommandQueue* CommandManager::GetCopyQueue()
	{
		return copy_queue_;
	}
	
	//------------------------------------------------------------------------------------------------------
	bool CommandManager::IsFenceComplete(uint64_t fence_value)
	{
		return GetQueue(static_cast<D3D12_COMMAND_LIST_TYPE>(fence_value >> 56))->IsFenceComplete(fence_value);
	}
	
	//------------------------------------------------------------------------------------------------------
	void CommandManager::WaitForIdleGPU()
	{
		graphics_queue_->WaitForIdle();
		compute_queue_->WaitForIdle();
		copy_queue_->WaitForIdle();
	}
	
	//------------------------------------------------------------------------------------------------------
	void CommandManager::WaitForFence(uint64_t fence_value)
	{
		GetQueue(static_cast<D3D12_COMMAND_LIST_TYPE>(fence_value >> 56))->WaitForFence(fence_value);
	}
}