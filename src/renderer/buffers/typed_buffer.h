#pragma once

#include "gpu_buffer.h"

namespace blowbox
{
	class TypedBuffer : public GpuBuffer
	{
	public:
		TypedBuffer();
		TypedBuffer(DXGI_FORMAT format);
		virtual void CreateDerivedViews() override;
	private:
		DXGI_FORMAT data_format_;
	};
}