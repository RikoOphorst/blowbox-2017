#pragma once

#include "gpu_buffer.h"

namespace blowbox
{
    /**
    * A ByteAddressBuffer allows a shader to access each element in the buffer
    * by a byte value. No clue what this is useful for yet. /shrug
    *
    * @brief Wrapper for random access buffers
    */
	class ByteAddressBuffer : public GpuBuffer
	{
	public:
        /** @brief Creates the descriptors (views) for this buffer. */
		virtual void CreateDerivedViews() override;
	};
}