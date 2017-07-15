#pragma once

#include "renderer/commands/command_context.h"
#include "renderer/pipeline_state.h"

namespace blowbox
{
    /**
    * Inherting from the CommandContext, this GraphicsContext allows you to create,
    * record and submit graphics command lists to the GPU. Use GraphicsContext::Begin()
    * to start a new context and submit it by either calling GraphicsContext::Finish()
    * or GraphicsContext::Flush() on it.
    *
    * @brief Wraps functionality for submitting graphics related command lists.
    */
	class GraphicsContext : public CommandContext
	{
	public:
        /**
        * @brief Begins a new GraphicsContext.
        * @param[in] name The name of the newly created GraphicsContext.
        * @returns A new GraphicsContext that can immediately be used to record commands.
        */
		static GraphicsContext& Begin(const eastl::wstring& name = L"");

        /**
        * @brief Clears a UAV resource.
        * @param[in] target The resource that should be cleared.
        */
		void ClearUAV(GpuBuffer& target);

        /**
        * @brief Clears a UAV resource.
        * @param[in] target The resource that should be cleared.
        */
		void ClearUAV(ColorBuffer& target);

        /**
        * @brief Clears a RTV resource.
        * @param[in] target The resource that should be cleared.
        */
		void ClearColor(ColorBuffer& target);

        /**
        * @brief Clears a DSV resource.
        * @param[in] target The resource that should be cleared.
        */
		void ClearDepth(DepthBuffer& target);

        /**
        * @brief Clears a DSV resource.
        * @param[in] target The resource that should be cleared.
        */
		void ClearStencil(DepthBuffer& target);

        /**
        * @brief Clears a DSV resource.
        * @param[in] target The resource that should be cleared.
        */
		void ClearDepthAndStencil(DepthBuffer& target);

        /**
        * @brief Begins a query.
        * @param[in] query_heap The query heap.
        * @param[in] type The query type.
        * @param[in] heap_index The heap index.
        */
		void BeginQuery(ID3D12QueryHeap* query_heap, D3D12_QUERY_TYPE type, UINT heap_index);

        /**
        * @brief Ends a query.
        * @param[in] query_heap The query heap.
        * @param[in] type The query type.
        * @param[in] heap_index The heap index.
        */
		void EndQuery(ID3D12QueryHeap* query_heap, D3D12_QUERY_TYPE type, UINT heap_index);

        /**
        * @brief Resolves a query.
        * @param[in] query_heap The query heap.
        * @param[in] type The query type.
        * @param[in] start_index The start heap index.
        * @param[in] num_queries The number of queries.
        * @param[in] dest_buffer The destination buffer.
        * @param[in] dest_buffer_offset The offset into the destination buffer.
        */
		void ResolveQueryData(ID3D12QueryHeap* query_heap, D3D12_QUERY_TYPE type, UINT start_index, UINT num_queries, ID3D12Resource* dest_buffer, UINT64 dest_buffer_offset);

        /**
        * @brief Sets the pipeline's root signature.
        * @param[in] root_signature The root signature that you want to set.
        */
		void SetRootSignature(const RootSignature& root_signature);

        /**
        * @brief Sets a number of render targets.
        * @param[in] num_rtvs The number of render targets that you want to set.
        * @param[in] rtvs The descriptor ids of the render targets that should be bound to the pipeline.
        */
		void SetRenderTargets(UINT num_rtvs, const UINT rtvs[]);

        /**
        * @brief Sets a number of render targets, and a depth stencil.
        * @param[in] num_rtvs The number of render targets that you want to set.
        * @param[in] rtvs The descriptor ids of the render targets that should be bound to the pipeline.
        * @param[in] dsv The descriptor id of the depth stencil buffer you want to bind.
        */
		void SetRenderTargets(UINT num_rtvs, const UINT rtvs[], UINT dsv);

        /**
        * @brief Sets a render target.
        * @param[in] rtv The descriptor id of the RTV that should be bound.
        */
		void SetRenderTarget(UINT rtv) { SetRenderTargets(1, &rtv); }

        /**
        * @brief Sets a render target and depth stencil.
        * @param[in] rtv The descriptor id of the RTV that should be bound.
        * @param[in] dsv The descriptor id of the DSV that should be bound.
        */
		void SetRenderTarget(UINT rtv, UINT dsv) { SetRenderTargets(1, &rtv, dsv); }

        /**
        * @brief Sets a depth stencil.
        * @param[in] dsv The descriptor id of the DSV that should be bound.
        * @remarks This also unbinds the current RTV, if any.
        */
		void SetDepthStencilTarget(UINT dsv) { SetRenderTargets(0, nullptr, dsv); }

        /**
        * @brief Sets the current viewport.
        * @param[in] vp The viewport that should be set.
        */
		void SetViewport(const D3D12_VIEWPORT& vp);

        /**
        * @brief Sets the current viewport.
        * @param[in] x The top left X coordinate.
        * @param[in] y The top left Y coordinate.
        * @param[in] w The width of the viewport.
        * @param[in] h The height of the viewport.
        * @param[in] min_depth The minimum depth value (closest).
        * @param[in] max_depth The maximum depth value (furthest).
        */
		void SetViewport(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT min_depth = 0.0f, FLOAT max_depth = 1.0f);

        /**
        * @brief Sets the current scissor rect.
        * @param[in] rect The scissor rect you want to set.
        */
		void SetScissor(const D3D12_RECT& rect);

        /**
        * @brief Sets the current scissor rect.
        * @param[in] left The left most coordinate of the rect.
        * @param[in] top The top most coordinate of the rect.
        * @param[in] right The right most coordinate of the rect.
        * @param[in] bottom The bottom most coordinate of the rect.
        */
		void SetScissor(UINT left, UINT top, UINT right, UINT bottom);

        /**
        * @brief Sets the current viewport and scissor rect.
        * @param[in] vp The viewport to be set.
        * @param[in] rect The scissor rect to be set.
        */
		void SetViewportAndScissor(const D3D12_VIEWPORT& vp, const D3D12_RECT& rect);

        /**
        * @brief Sets the current viewport and scissor rect - identically. (!)
        * @param[in] x The top left X coordinate.
        * @param[in] y The top left Y coordinate.
        * @param[in] w The width of the rect to be set.
        * @param[in] h The height of the rect to be set.
        */
		void SetViewportAndScissor(UINT x, UINT y, UINT w, UINT h);

        /**
        * @brief Sets the stencil reference value.
        * @param[in] stencil_ref The stencil reference value.
        */
		void SetStencilRef(UINT stencil_ref);

        /**
        * @brief Sets the blend factor.
        * @param[in] colorRGBA The blend factor.
        */
		void SetBlendFactor(float colorRGBA[]);
        
        /**
        * @brief Sets the primitive topology.
        * @param[in] topology The primitive topology.
        */
		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

        /**
        * @brief Sets the pipeline state.
        * @param[in] pso The PSO that should be bound to the pipeline.
        */
		void SetPipelineState(const GraphicsPSO& pso);

        /**
        * @brief Sets a number of root constants.
        * @param[in] root_index The index in the root signature that the constants should be put at.
        * @param[in] num_constants The number of constants to be set.
        * @param[in] constants The constants that should be set.
        */
		void SetConstants(UINT root_index, UINT num_constants, const void* constants);

        /**
        * @brief Sets a root constant.
        * @param[in] root_index The index in the root signature that the constants should be put at.
        * @param[in] x The first constant data.
        */
		void SetConstants(UINT root_index, Param32Bit x);

        /**
        * @brief Sets a root constant.
        * @param[in] root_index The index in the root signature that the constants should be put at.
        * @param[in] x The first constant data.
        * @param[in] y The second constant data.
        */
		void SetConstants(UINT root_index, Param32Bit x, Param32Bit y);

        /**
        * @brief Sets a root constant
        * @param[in] root_index The index in the root signature that the constants should be put at.
        * @param[in] x The first constant data.
        * @param[in] y The second constant data.
        * @param[in] z The third constant data.
        */
		void SetConstants(UINT root_index, Param32Bit x, Param32Bit y, Param32Bit z);

        /**
        * @brief Sets a root constant.
        * @param[in] root_index The index in the root signature that the constants should be put at.
        * @param[in] x The first constant data.
        * @param[in] y The second constant data.
        * @param[in] z The third constant data.
        * @param[in] w The fourth constant data.
        */
		void SetConstants(UINT root_index, Param32Bit x, Param32Bit y, Param32Bit z, Param32Bit w);

        /**
        * @brief Sets a root constant buffer.
        * @param[in] root_index The index in the root signature that the buffer should be put at.
        * @param[in] cbv The GPU address where the buffer exists.
        */
		void SetConstantBuffer(UINT root_index, D3D12_GPU_VIRTUAL_ADDRESS cbv);

        /**
        * @brief Binds a root SRV buffer to the pipeline.
        * @param[in] root_index The index in the root signature that the buffer should be put at.
        * @param[in] srv The descriptor ID of the SRV that should be bound to the pipeline.
        * @param[in] offset If an offset into the SRV buffer should be used, this is the place to do it.
        */
		void SetBufferSRV(UINT root_index, GpuBuffer& srv, UINT offset = 0);

        /**
        * @brief Binds a root UAV buffer to the pipeline.
        * @param[in] root_index The index in the root signature that the buffer should be put at.
        * @param[in] uav The descriptor ID of the UAV that should be bound to the pipeline.
        * @param[in] offset If an offset into the UAV buffer should be used, this is the place to do it.
        */
		void SetBufferUAV(UINT root_index, GpuBuffer& uav, UINT offset = 0);

        /**
        * @brief Bind a descriptortable to the pipeline.
        * @param[in] root_index The index in the root signature that the buffer should be put at.
        * @param[in] first_handle The first handle to a descriptor in a descriptor heap from where the descriptor table should start.
        */
		void SetDescriptorTable(UINT root_index, D3D12_GPU_DESCRIPTOR_HANDLE first_handle);

        /**
        * @brief Binds an index buffer to the pipeline.
        * @param[in] ibv The index buffer view that should be bound.
        */
		void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& ibv);

        /**
        * @brief Binds a vertex buffer to the pipeline.
        * @param[in] slot The slot at which the buffer should be bound, usually 0.
        * @param[in] vbv The vertex buffer view that should be bound.
        */
		void SetVertexBuffer(UINT slot, const D3D12_VERTEX_BUFFER_VIEW& vbv);

        /**
        * @brief Binds a number of vertex buffers to the pipeline.
        * @param[in] start_slot The slot at which the buffers should be bound, usually 0.
        * @param[in] count The number of vertex buffers to bind.
        * @param[in] vbvs The vertex buffer views that should be bound.
        */
		void SetVertexBuffers(UINT start_slot, UINT count, const D3D12_VERTEX_BUFFER_VIEW vbvs[]);

        /**
        * @brief Issues a non-instanced non-indexed draw call.
        * @param[in] vertex_count Number of vertices that should be drawn.
        * @param[in] vertex_start_offset The offset into the vertex buffer from which the vertices should start counting, usually 0.
        */
		void Draw(UINT vertex_count, UINT vertex_start_offset = 0);

        /**
        * @brief Issues an non-instanced indexed draw call.
        * @param[in] index_count Number of indices that should be drawn.
        * @param[in] start_index_location The offset into the index buffer from which the indices should start counting, usually 0.
        * @param[in] base_vertex_location The offset into the vertex buffer from which the vertices should start counting, usually 0.
        */
		void DrawIndexed(UINT index_count, UINT start_index_location = 0, INT base_vertex_location = 0);

        /**
        * @brief Issues an instanced non-indexed draw call.
        * @param[in] vertex_count_per_instance Number of vertices per instance.
        * @param[in] instance_count Number of instances that should be drawn.
        * @param[in] start_vertex_location The offset into the vertex buffer from which the vertices should start counting, usually 0.
        * @param[in] start_instance_location The offset into the vertex buffer from which the per instance vertex buffer should start counting, usually 0.
        */
		void DrawInstanced(UINT vertex_count_per_instance, UINT instance_count, UINT start_vertex_location = 0, UINT start_instance_location = 0);

        /**
        * @brief Issues an instanced indexed draw call.
        * @param[in] index_count_per_instance Number of vertices per instance.
        * @param[in] instance_count Number of instances that should be drawn.
        * @param[in] start_index_location The offset into the index buffer from which the indices should start counting, usually 0.
        * @param[in] start_vertex_location The offset into the vertex buffer from which the vertices should start counting, usually 0.
        * @param[in] start_instance_location The offset into the vertex buffer from which the per instance vertex buffer should start counting, usually 0.
        */
		void DrawIndexedInstanced(UINT index_count_per_instance, UINT instance_count, UINT start_index_location, INT start_vertex_location, UINT start_instance_location);

        /**
        * @brief Issues an indirect draw buffer.
        * @param[in] argument_buffer Argument buffer.
        * @param[in] argument_buffer_offset Argument buffer offset.
        */
		void DrawIndirect(GpuBuffer& argument_buffer, UINT argument_buffer_offset = 0);
	};
}