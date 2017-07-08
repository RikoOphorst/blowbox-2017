#pragma once

#include "gpu_buffer.h"
#include "byte_address_buffer.h"

namespace blowbox
{
	class CommandContext;

	class StructuredBuffer : public GpuBuffer
	{
	public:
		virtual void Destroy() override;

		virtual void CreateDerivedViews() override;

		ByteAddressBuffer& GetCounterBuffer() { return counter_buffer_; };
		const ByteAddressBuffer& GetCounterBuffer() const { return counter_buffer_; };

		const UINT& GetCounterSRV(CommandContext& context);
		const UINT& GetCounterUAV(CommandContext& context);

	private:
		ByteAddressBuffer counter_buffer_;
	};
}