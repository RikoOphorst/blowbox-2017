#pragma once

namespace blowbox
{
	class CommandAllocatorPool;

	class CommandQueue
	{
	public:
		CommandQueue(D3D12_COMMAND_LIST_TYPE type);
		~CommandQueue();

		bool IsFenceComplete(uint64_t fence_value);
		uint64_t IncrementFence(); 
		void WaitForFence(uint64_t fence_value);
		void WaitForIdle();
		void StallForFence(uint64_t fence_value);
		void StallForProducer(CommandQueue* producer);

		uint64_t ExecuteCommandList(ID3D12CommandList* list);

		ID3D12CommandQueue* Get() { return queue_; };
		ID3D12CommandAllocator* RequestAllocator();
		void DiscardAllocator(uint64_t fence_value, ID3D12CommandAllocator* allocator);

	private:
		D3D12_COMMAND_LIST_TYPE type_;
		ID3D12CommandQueue* queue_;
		CommandAllocatorPool* allocator_pool_;

		ID3D12Fence* fence_;
		uint64_t last_completed_fence_value_;
		uint64_t next_fence_value_;
		HANDLE fence_event_handle_;
	};
}