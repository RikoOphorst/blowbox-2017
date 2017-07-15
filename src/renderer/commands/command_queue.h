#pragma once

#include "renderer/d3d12_includes.h"

namespace blowbox
{
	class CommandAllocatorPool;

    /**
    * The CommandQueue wraps all functionality from a ID3D12CommandQueue.
    * It handles the CommandQueue in a way that allows for smart CPU/GPU
    * synchronization. It generates a fence value specifically for this
    * type of CommandQueue. See remarks for an explanation of fence values.
    * Additionally, the CommandQueue manages a CommandAllocatorPool for
    * the type of CommandQueue that this is. If you need a CommandAllocator,
    * you should request it through the CommandQueue.
    *
    * @brief Manages a ID3D12CommandQueue.
    * @remarks Remember, a fence_value is in fact two values. The first (highest-order) 8 bits represent the type of CommandQueue that generated the fence value. 
    *          The remaining 56 bits are the actual value of the fence point. A fence point is essentially a point in time. After a command list is submitted, a
    *          fence point is generated for the correct command queue. That means that after every command list submission, a new point in time is "generated".
    */
	class CommandQueue
	{
	public:
        /**
        * @brief Constructs a CommandQueue of a certain type.
        * @param[in] type The type of command queue that should be created.
        */
		CommandQueue(D3D12_COMMAND_LIST_TYPE type);

        /** @brief Destructs the CommandQueue. */
		~CommandQueue();

        /** 
        * @brief Checks if a certain fence value has been passed. 
        * @param[in] fence_value The fence value you want to check if it was passed.
        * @remarks Remember, a fence_value is in fact two values. The first (highest-order) 8 bits represent the type of CommandQueue that generated the fence value. 
        *          The remaining 56 bits are the actual value of the fence point. A fence point is essentially a point in time. After a command list is submitted, a
        *          fence point is generated for the correct command queue. That means that after every command list submission, a new point in time is "generated".
        */
		bool IsFenceComplete(uint64_t fence_value);

        /** @brief Increments the current fence and signals the CommandQueue. */
		uint64_t IncrementFence(); 

        /**
        * @brief Makes the CommandQueue wait for a given fence value.
        * @param[in] fence_value The fence value you want to wait on.
        * @remarks Remember, a fence_value is in fact two values. The first (highest-order) 8 bits represent the type of CommandQueue that generated the fence value. 
        *          The remaining 56 bits are the actual value of the fence point. A fence point is essentially a point in time. After a command list is submitted, a
        *          fence point is generated for the correct command queue. That means that after every command list submission, a new point in time is "generated".
        */
		void WaitForFence(uint64_t fence_value);

        /** @brief Makes the CommandQueue wait for until it is completely empty (i.e. the GPU finished all the work submitted to this CommandQueue). */
		void WaitForIdle();

        /** 
        * @brief Makes this CommandQueue wait with execution until the given fence value is hit. See remarks!
        * @param[in] fence_value The fence value you want this CommandQueue to wait for.
        * @remarks Remember, a fence_value is in fact two values. The first (highest-order) 8 bits represent the type of CommandQueue that generated the fence value. 
        *          The remaining 56 bits are the actual value of the fence point. A fence point is essentially a point in time. After a command list is submitted, a
        *          fence point is generated for the correct command queue. That means that after every command list submission, a new point in time is "generated".
        */
		void StallForFence(uint64_t fence_value);

        /**
        * @brief Makes this CommandQueue wait with execution until a given CommandQueue is finished executing work.
        * @param[in] producer The other CommandQueue that has to be waited on.
        */
		void StallForProducer(CommandQueue* producer);

        /**
        * @brief Submits a command list to the CommandQueue.
        * @param[in] list The list you want to submit
        * @returns A fence value that, once passed, means that the submitted command list has been completed.
        * @remarks Remember, a fence_value is in fact two values. The first (highest-order) 8 bits represent the type of CommandQueue that generated the fence value. 
        *          The remaining 56 bits are the actual value of the fence point. A fence point is essentially a point in time. After a command list is submitted, a
        *          fence point is generated for the correct command queue. That means that after every command list submission, a new point in time is "generated".
        */
		uint64_t ExecuteCommandList(ID3D12CommandList* list);

        /** @returns The underlying ID3D12CommandQueue. */
		ID3D12CommandQueue* Get() { return queue_; };

        /** 
        * @brief Requests a ID3D12CommandAllocator from the underlying CommandAllocatorPool.
        * @returns A ID3D12CommandAllocator which is ready for use.
        */
		ID3D12CommandAllocator* RequestAllocator();

        /**
        * @brief Discards a ID3D12CommandAllocator back into the CommandAllocatorPool as soon as the given fence_value has been passed.
        * @param[in] fence_value The fence value that needs to be passed before the ID3D12CommandAllocator can be repurposed.
        * @param[in] allocator The allocator that you want to release back into the CommandAllocatorPool.
        * @remarks Remember, a fence_value is in fact two values. The first (highest-order) 8 bits represent the type of CommandQueue that generated the fence value. 
        *          The remaining 56 bits are the actual value of the fence point. A fence point is essentially a point in time. After a command list is submitted, a
        *          fence point is generated for the correct command queue. That means that after every command list submission, a new point in time is "generated".
        */
		void DiscardAllocator(uint64_t fence_value, ID3D12CommandAllocator* allocator);

	private:
		D3D12_COMMAND_LIST_TYPE type_;          //!< The type of CommandQueue this is.
		ID3D12CommandQueue* queue_;             //!< The actual ID3D12CommandQueue.
		CommandAllocatorPool* allocator_pool_;  //!< The CommandAllocatorPool that is controlled by this CommandQueue.

		ID3D12Fence* fence_;                    //!< The ID3D12Fence object that we use to synchronize the CPU with the GPU.
		uint64_t last_completed_fence_value_;   //!< The fence value that was most recently completed by the GPU.
		uint64_t next_fence_value_;             //!< The next fence value that should be used when we want to submit a new unit of work.
		HANDLE fence_event_handle_;             //!< A handle to an event that we use to synchronize the CPU with the GPU.
	};
}