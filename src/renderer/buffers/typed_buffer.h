#pragma once

#include "gpu_buffer.h"

namespace blowbox
{
    /**
    * This type of buffer acts similarly to a StructuredBuffer, except
    * every element in the buffer is formatted in the form of a DXGI_FORMAT.
    *
    * @brief Wrapper for formatted buffer types.
    */
	class TypedBuffer : public GpuBuffer
	{
	public:
        /** @brief Constructs a TypedBuffer. */
		TypedBuffer();

        /**
        * @brief Constructs a TypedBuffer with a given format
        * @param[in] format The format of each element in the buffer.
        */
		TypedBuffer(DXGI_FORMAT format);

        /** @brief Creates the descriptor views (descriptors) for this TypedBuffer. */
		virtual void CreateDerivedViews() override;
	private:
		DXGI_FORMAT data_format_; //!< The format of each element in the typed buffer.
	};
}