#pragma once

#include "renderer/d3d12_includes.h"

#include "util/shared_ptr.h"
#include "util/weak_ptr.h"

namespace blowbox
{
	class Device;
	class CommandQueue;
	class CommandList;

    /**
    * The CommandManager manages all the different CommandQueues in the engine.
    * At any time, there are three different queues: a graphics queue, a compute
    * queue and a copy queue. A big convenience that the CommandManager offers
    * is the CPU/GPU synchronization features. Fence values are constructed in
    * a unique way. The first (highest order) 8 bits are reserved for data that
    * specifies the type of command queue the fence value is for. So by calling
    * CommandManager::WaitForFence() and passing in a Blowbox generated fence
    * value, it will automatically figure out which command queue needs to be
    * waited on. This way, CPU/GPU synchronization becomes tentalizingly easy.
    * That is the main purpose of the CommandManager.
    * 
    * @brief Manages all the CommandQueues in the application.
    */
	class CommandManager
	{
	public:
        /** @brief Constructs the CommandManager. */
		CommandManager();
        /** @brief Destructs the CommandManager. */
		~CommandManager();

        /** @brief Starts up the CommandManager. */
		void Startup();

        /**
        * @brief Creates a ID3D12GraphicsCommandList with an ID3D12CommandAllocator that is ready for use.
        * @param[in] type The type of ID3D12GraphicsCommandList that should be created.
        * @param[out] out_list The generated ID3D12GraphicsCommandList will be output here.
        * @param[out] out_allocator The generated ID3D12CommandAllocator willl be output here.
        */
		void CreateCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** out_list, ID3D12CommandAllocator** out_allocator);

        /**
        * @returns The CommandQueue of a given type.
        * @param[in] type The type of CommandQueue you want.
        */
		CommandQueue* GetQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

        /** @returns Returns the main CommandQueue. */
		CommandQueue* GetCommandQueue();

        /** @returns Returns the graphics CommandQueue. */
		CommandQueue* GetGraphicsQueue();

        /** @returns Returns the compute CommandQueue. */
		CommandQueue* GetComputeQueue();

        /** @returns Returns the copy CommandQueue. */
		CommandQueue* GetCopyQueue();

        /**
        * @brief Checks if a given fence value (point in time) has been passed by the corresponding command queue.
        * @param[in] fence_value The fence value that you want to check whether it has been passed.
        * @remarks Remember, a fence_value is in fact two values. The first (highest-order) 8 bits represent the type of CommandQueue that generated the fence value. 
        *          The remaining 56 bits are the actual value of the fence point. A fence point is essentially a point in time. After a command list is submitted, a
        *          fence point is generated for the correct command queue. That means that after every command list submission, a new point in time is "generated".
        */
		bool IsFenceComplete(uint64_t fence_value);

        /** @brief This function is blocking until all pending work in the CommandQueues is done executing. */
		void WaitForIdleGPU();

        /**
        * @brief This function is blocking until a given fence point is hit by the corresponding CommandQueue.
        * @param[in] fence_value The fence value that has to be waited for.
        * @remarks Remember, a fence_value is in fact two values. The first (highest-order) 8 bits represent the type of CommandQueue that generated the fence value. 
        *          The remaining 56 bits are the actual value of the fence point. A fence point is essentially a point in time. After a command list is submitted, a
        *          fence point is generated for the correct command queue. That means that after every command list submission, a new point in time is "generated".
        */
		void WaitForFence(uint64_t fence_value);

	private:
		CommandQueue* graphics_queue_;  //!< The graphics CommandQueue.
		CommandQueue* compute_queue_;   //!< The compute CommandQueue.
		CommandQueue* copy_queue_;      //!< The copy CommandQueue.
	};
}