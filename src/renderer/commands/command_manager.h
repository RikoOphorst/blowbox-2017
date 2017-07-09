#pragma once

#include "renderer/d3d12_includes.h"

namespace blowbox
{
	class Device;
	class CommandQueue;
	class CommandList;

	class CommandManager
	{
	public:
		CommandManager();
		~CommandManager();

		void Startup(Device* device);

		void CreateCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** out_list, ID3D12CommandAllocator** out_allocator);

		CommandQueue* GetQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
		CommandQueue* GetCommandQueue();
		CommandQueue* GetGraphicsQueue();
		CommandQueue* GetComputeQueue();
		CommandQueue* GetCopyQueue();

		bool IsFenceComplete(uint64_t fence_value);
		void WaitForIdleGPU();
		void WaitForFence(uint64_t fence_value);

	private:
		Device* device_;

		CommandQueue* graphics_queue_;
		CommandQueue* compute_queue_;
		CommandQueue* copy_queue_;
	};
}