#pragma once

#include "util/vector.h"
#include "util/queue.h"
#include "util/utility.h"
#include "renderer/d3d12_includes.h"

namespace blowbox
{
    /**
    * The CommandAllocatorPool holds a pool of command allocators.
    * It dynamically creates more CommandAllocators when they run
    * out, and releases them back into the pool of available all-
    * ocators when they are done processing on the GPU.
    *
    * @brief Controls a pool of allocators of a certain type.
    */
	class CommandAllocatorPool
	{
	public:
        /**
        * @brief Constructs a CommandAllocatorPool with CommandAllocators of a given type.
        * @param[in] type The type of CommandAllocators that this pool holds.
        */
		CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type);

        /** @brief Destructs the CommandAllocatorPool. */
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

        /** @returns The number of command allocators in the pool currently. */
		inline size_t GetPoolSize() { return allocator_pool_.size(); }

	private:
		D3D12_COMMAND_LIST_TYPE type_;                                          //!< The type of command allocator pool
		Vector<ID3D12CommandAllocator*> allocator_pool_;                        //!< All command allocators exist within this pool
        Queue<Pair<uint64_t, ID3D12CommandAllocator*>> available_allocators_;   //!< Allocators that have been discarded, are put into this Queue, accomponied by their expiry fence point.
	};
}