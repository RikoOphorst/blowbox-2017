#pragma once

#include "util/vector.h"
#include "util/queue.h"
#include "util/utility.h"
#include "renderer/d3d12_includes.h"

namespace blowbox
{
	class CommandAllocatorPool
	{
	public:
		CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type);
		~CommandAllocatorPool();

		/**
		* @brief This requests an allocator that will be available past the given fence point.
		* @param[in] completed_fence_value	Fence point (point in time, essentially) at which the given allocator should be available
		*/
		ID3D12CommandAllocator* RequestAllocator(uint64_t completed_fence_value);

		/**
		* @brief Discards a given allocator. It will be available after a given fence point (point in time, essentially)
		* @param[in] fence_value	The fence point at which the discarded allocator is available again
		* @param[in] allocator		The allocator that is getting discarded
		*/
		void DiscardAllocator(uint64_t fence_value, ID3D12CommandAllocator* allocator);

		inline size_t GetPoolSize() { return allocator_pool_.size(); }

	private:
		D3D12_COMMAND_LIST_TYPE type_;
		Vector<ID3D12CommandAllocator*> allocator_pool_;
        Queue<Pair<uint64_t, ID3D12CommandAllocator*>> available_allocators_;
	};
}