#pragma once

#include "gpu_buffer.h"
#include "byte_address_buffer.h"

namespace blowbox
{
	class CommandContext;

    /**
    * This buffer allows you to easily create structured buffers. These are
    * types of buffers which are partitioned into a number of elements. All
    * elements are the exact same types, and repeating through the buffer.
    * You can essentially see this is as the array equivalent of buffers.
    *
    * @brief Wrapper for StructuredBuffer types
    */
	class StructuredBuffer : public GpuBuffer
	{
	public:
        /** @brief Destroys the StructuredBuffer. */
		virtual void Destroy() override;

        /** @brief Creates the necessary descriptors (views) for this StructuredBuffer. */
		virtual void CreateDerivedViews() override;

        /** @returns The counter buffer attached to this StructuredBuffer, used for dynamically resizing the StructuredBuffer. */
		ByteAddressBuffer& GetCounterBuffer() { return counter_buffer_; };

        /** @returns The counter buffer attached to this StructuredBuffer, used for dynamically resizing the StructuredBuffer. */
		const ByteAddressBuffer& GetCounterBuffer() const { return counter_buffer_; };

        /** 
        * @returns The Shader Resource View for the Counter buffer.
        * @remarks This automatically transitions the underlying counter buffer to the D3D12_RESOURCE_STATE_GENERIC_READ state.
        */
		const UINT& GetCounterSRV(CommandContext& context);

        /**
        * @returns The Unordered Access View for the Counter buffer.
        * @remarks This automatically transitions the underlying counter buffer to the D3D12_RESOURCE_STATE_UNORDERED_ACCESS state.
        */
		const UINT& GetCounterUAV(CommandContext& context);

	private:
		ByteAddressBuffer counter_buffer_; //!< The counter buffer for this StructuredBuffer, used to keep track of how large the StructuredBuffer is.
	};
}