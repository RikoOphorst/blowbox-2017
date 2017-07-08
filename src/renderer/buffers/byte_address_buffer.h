#pragma once

#include "gpu_buffer.h"

namespace blowbox
{
	class ByteAddressBuffer : public GpuBuffer
	{
	public:
		virtual void CreateDerivedViews() override;
	};
}