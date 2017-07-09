#pragma once

#include "core/eastl_required.h"
#include <EASTL/string.h>

#include "renderer/buffers/gpu_resource.h"
#include "renderer/buffers/gpu_buffer.h"
#include "renderer/buffers/structured_buffer.h"
#include "renderer/buffers/byte_address_buffer.h"
#include "renderer/buffers/depth_buffer.h"
#include "renderer/buffers/color_buffer.h"
#include "renderer/buffers/upload_buffer.h"

#define COMMAND_CONTEXT_BARRIER_BUFFER_SIZE 16

namespace blowbox
{
	struct Param32Bit
	{
		Param32Bit(FLOAT f_) : f(f_) {}
		Param32Bit(UINT u_) : u(u_) {}
		Param32Bit(INT i_) : i(i_) {}

		void operator=(FLOAT f_) { f = f_; }
		void operator=(UINT u_) { u = u_; }
		void operator=(INT i_) { i = i_; }

		union
		{
			FLOAT f;
			UINT u;
			INT i;
		};
	};

	class GraphicsContext;
	class ComputeContext;

	struct NonCopyable
	{
		NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};

	class CommandContext : NonCopyable
	{
		friend class CommandContextManager;
	private:
		CommandContext(D3D12_COMMAND_LIST_TYPE type);

		void Reset();

	public:
		~CommandContext();

		static CommandContext& Begin(const eastl::wstring& name = L"");
		GraphicsContext& GetGraphicsContext();
		ComputeContext& GetComputeContext();

		void Initialize();

		static void InitializeTexture(GpuResource& dest_resource, UINT num_subresources, D3D12_SUBRESOURCE_DATA subresource_data[]);
		static void InitializeBuffer(GpuResource& dest_resource, const void* data, UINT num_bytes, bool use_offset = false, UINT offset = 0);

		void CopyBuffer(GpuResource& dest_resource, UploadBuffer& source_resource);
		void CopyBuffer(GpuResource& dest_resource, GpuResource& source_resource);
		void CopyBufferRegion(GpuResource& dest_resource, UINT dest_offset, GpuResource& source_resource, UINT source_offset, UINT num_bytes);
		void CopySubresource(GpuResource& dest_resource, UINT dest_resource_index, GpuResource& source_resource, UINT source_sub_index);
		void CopyCounter(GpuResource& dest_resource, UINT dest_offset, StructuredBuffer& source_resource);
		void ResetCounter(StructuredBuffer& buffer, uint32_t value = 0);

		void WriteBuffer(GpuResource& dest_resource, UINT dest_offset, const void* data, UINT num_bytes);
		void FillBuffer(GpuResource& dest_resource, UINT dest_offset, Param32Bit value, UINT num_bytes);

		uint64_t Flush(bool wait_for_completion = false);
		uint64_t Finish(bool wait_for_completion = false);

		void TransitionResource(GpuResource& resource, const D3D12_RESOURCE_STATES& new_state, bool flush_immediate = false);
		void BeginResourceTransition(GpuResource& resource, const D3D12_RESOURCE_STATES& new_state, bool flush_immediate = false);
		void InsertUAVBarrier(GpuResource& resource, bool flush_immediate = false);
		void InsertAliasBuffer(GpuResource& before, GpuResource& after, bool flush_immediate = false);
		void FlushResourceBarriers();

		void InsertTimeStamp(ID3D12QueryHeap* query_heap, UINT query_idx);
		void ResolveTimeStamps(ID3D12Resource* read_back_heap, ID3D12QueryHeap* query_heap, UINT num_queries);

		void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12DescriptorHeap* heap);
		void SetDescriptorHeaps(UINT heap_count, D3D12_DESCRIPTOR_HEAP_TYPE heap_types[], ID3D12DescriptorHeap* heaps[]);

		void SetPredication(ID3D12Resource* buffer, UINT64 buffer_offset, D3D12_PREDICATION_OP op);

		ID3D12GraphicsCommandList* Get();

	protected:
		void BindDescriptorHeaps();

	protected:
		eastl::wstring name_;
        void SetName(const eastl::wstring& name) { list_->SetName(name.c_str()); name_ = name; }

	protected:
		D3D12_COMMAND_LIST_TYPE type_;
		ID3D12GraphicsCommandList* list_;
		ID3D12CommandAllocator* allocator_;

		UINT num_flushable_barriers_;
		D3D12_RESOURCE_BARRIER resource_barrier_buffer_[COMMAND_CONTEXT_BARRIER_BUFFER_SIZE];

		ID3D12DescriptorHeap* descriptor_heaps_[4];

		ID3D12RootSignature* graphics_root_signature_;
		ID3D12PipelineState* graphics_pipeline_state_;
		ID3D12RootSignature* compute_root_signature_;
		ID3D12PipelineState* compute_pipeline_state_;
	};
}