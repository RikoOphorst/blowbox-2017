#pragma once

#include "renderer/commands/command_context.h"
#include "renderer/pipeline_state.h"

namespace blowbox
{
	class GraphicsContext : public CommandContext
	{
	public:
		static GraphicsContext& Begin(const eastl::wstring& name = L"");

		void ClearUAV(GpuBuffer& target);
		void ClearUAV(ColorBuffer& target);
		void ClearColor(ColorBuffer& target);
		void ClearDepth(DepthBuffer& target);
		void ClearStencil(DepthBuffer& target);
		void ClearDepthAndStencil(DepthBuffer& target);

		void BeginQuery(ID3D12QueryHeap* query_heap, D3D12_QUERY_TYPE type, UINT heap_index);
		void EndQuery(ID3D12QueryHeap* query_heap, D3D12_QUERY_TYPE type, UINT heap_index);
		void ResolveQueryData(ID3D12QueryHeap* query_heap, D3D12_QUERY_TYPE type, UINT start_index, UINT num_queries, ID3D12Resource* dest_buffer, UINT64 dest_buffer_offset);

		void SetRootSignature(const RootSignature& root_signature);

		void SetRenderTargets(UINT num_rtvs, const UINT rtvs[]);
		void SetRenderTargets(UINT num_rtvs, const UINT rtvs[], UINT dsv);
		void SetRenderTarget(UINT rtv) { SetRenderTargets(1, &rtv); }
		void SetRenderTarget(UINT rtv, UINT dsv) { SetRenderTargets(1, &rtv, dsv); }
		void SetDepthStencilTarget(UINT dsv) { SetRenderTargets(0, nullptr, dsv); }

		void SetViewport(const D3D12_VIEWPORT& vp);
		void SetViewport(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT min_depth = 0.0f, FLOAT max_depth = 1.0f);
		void SetScissor(const D3D12_RECT& rect);
		void SetScissor(UINT left, UINT top, UINT right, UINT bottom);
		void SetViewportAndScissor(const D3D12_VIEWPORT& vp, const D3D12_RECT& rect);
		void SetViewportAndScissor(UINT x, UINT y, UINT w, UINT h);
		void SetStencilRef(UINT stencil_ref);
		void SetBlendFactor(float colorRGBA[]);
		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

		void SetPipelineState(const GraphicsPSO& PSO);
		void SetConstants(UINT root_index, UINT num_constants, const void* constants);
		void SetConstants(UINT root_index, Param32Bit x);
		void SetConstants(UINT root_index, Param32Bit x, Param32Bit y);
		void SetConstants(UINT root_index, Param32Bit x, Param32Bit y, Param32Bit z);
		void SetConstants(UINT root_index, Param32Bit x, Param32Bit y, Param32Bit z, Param32Bit w);
		void SetConstantBuffer(UINT root_index, D3D12_GPU_VIRTUAL_ADDRESS cbv);
		void SetBufferSRV(UINT root_index, GpuBuffer& srv, UINT offset = 0);
		void SetBufferUAV(UINT root_index, GpuBuffer& uav, UINT offset = 0);
		void SetDescriptorTable(UINT root_index, D3D12_GPU_DESCRIPTOR_HANDLE first_handle);

		void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& ibv);
		void SetVertexBuffer(UINT slot, const D3D12_VERTEX_BUFFER_VIEW& vbv);
		void SetVertexBuffers(UINT start_slot, UINT count, const D3D12_VERTEX_BUFFER_VIEW vbvs[]);

		void Draw(UINT vertex_count, UINT vertex_start_offset = 0);
		void DrawIndexed(UINT index_count, UINT start_index_location = 0, INT base_vertex_location = 0);
		void DrawInstanced(UINT vertex_count_per_instance, UINT instance_count, UINT start_vertex_location = 0, UINT start_instance_location = 0);
		void DrawIndexedInstanced(UINT index_count_per_instance, UINT instance_count, UINT start_index_location, INT start_vertex_location, UINT start_instance_location);
		void DrawIndirect(GpuBuffer& argument_buffer, UINT argument_buffer_offset = 0);
	};
}