#include "graphics_context.h"

#include "core/get.h"
#include "renderer/descriptor_heap.h"
#include "renderer/root_signature.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	GraphicsContext& GraphicsContext::Begin(const eastl::wstring& name)
	{
		return CommandContext::Begin(name).GetGraphicsContext();
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::ClearUAV(GpuBuffer& target)
	{
		const UINT clear_color[4] = {};
		list_->ClearUnorderedAccessViewUint(Get::CbvSrvUavHeap()->GetGPUDescriptorById(target.GetUAV()), Get::CbvSrvUavHeap()->GetCPUDescriptorById(target.GetUAV()), target, clear_color, 0, nullptr);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::ClearUAV(ColorBuffer& target)
	{
		const float* clear_color = target.GetClearColor();
		CD3DX12_RECT clear_rect(0, 0, static_cast<LONG>(target.GetWidth()), static_cast<LONG>(target.GetHeight()));
		list_->ClearUnorderedAccessViewFloat(Get::CbvSrvUavHeap()->GetGPUDescriptorById(target.GetUAV()), Get::CbvSrvUavHeap()->GetCPUDescriptorById(target.GetUAV()), target, clear_color, 1, &clear_rect);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::ClearColor(ColorBuffer& target)
	{
		list_->ClearRenderTargetView(Get::RtvHeap()->GetCPUDescriptorById(target.GetRTV()), target.GetClearColor(), 0, nullptr);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::ClearDepth(DepthBuffer& target)
	{
		list_->ClearDepthStencilView(Get::DsvHeap()->GetCPUDescriptorById(target.GetDSV()), D3D12_CLEAR_FLAG_DEPTH, target.GetDepthClearValue(), target.GetStencilClearValue(), 0, nullptr);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::ClearStencil(DepthBuffer& target)
	{
		list_->ClearDepthStencilView(Get::DsvHeap()->GetCPUDescriptorById(target.GetDSV()), D3D12_CLEAR_FLAG_STENCIL, target.GetDepthClearValue(), target.GetStencilClearValue(), 0, nullptr);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::ClearDepthAndStencil(DepthBuffer& target)
	{
		list_->ClearDepthStencilView(Get::DsvHeap()->GetCPUDescriptorById(target.GetDSV()), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, target.GetDepthClearValue(), target.GetStencilClearValue(), 0, nullptr);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::BeginQuery(ID3D12QueryHeap * query_heap, D3D12_QUERY_TYPE type, UINT heap_index)
	{
		list_->BeginQuery(query_heap, type, heap_index);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::EndQuery(ID3D12QueryHeap * query_heap, D3D12_QUERY_TYPE type, UINT heap_index)
	{
		list_->EndQuery(query_heap, type, heap_index);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::ResolveQueryData(ID3D12QueryHeap* query_heap, D3D12_QUERY_TYPE type, UINT start_index, UINT num_queries, ID3D12Resource* dest_buffer, UINT64 dest_buffer_offset)
	{
		list_->ResolveQueryData(query_heap, type, start_index, num_queries, dest_buffer, dest_buffer_offset);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetRootSignature(const RootSignature& root_signature)
	{
		if (root_signature.Get() == graphics_root_signature_)
			return;

		graphics_root_signature_ = root_signature.Get();
		list_->SetGraphicsRootSignature(root_signature.Get());
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetRenderTargets(UINT num_rtvs, const UINT rtvs[])
	{
		assert(num_rtvs <= 16); // arbitrary rtv limit, ONLY because of handles array size
		
		D3D12_CPU_DESCRIPTOR_HANDLE handles[16];
		for (UINT i = 0; i < num_rtvs; i++)
		{
			handles[i] = Get::RtvHeap()->GetCPUDescriptorById(rtvs[i]);
		}

		list_->OMSetRenderTargets(num_rtvs, handles, false, nullptr);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetRenderTargets(UINT num_rtvs, const UINT rtvs[], UINT dsv)
	{
		assert(num_rtvs <= 16); // arbitrary rtv limit, ONLY because of handles array size

		D3D12_CPU_DESCRIPTOR_HANDLE handles[16];
		for (UINT i = 0; i < num_rtvs; i++)
		{
			handles[i] = Get::RtvHeap()->GetCPUDescriptorById(rtvs[i]);
		}

		list_->OMSetRenderTargets(num_rtvs, handles, false, &Get::DsvHeap()->GetCPUDescriptorById(dsv));
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetViewport(const D3D12_VIEWPORT& vp)
	{
		list_->RSSetViewports(1, &vp);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetViewport(FLOAT x, FLOAT y, FLOAT w, FLOAT h, FLOAT min_depth, FLOAT max_depth)
	{
		D3D12_VIEWPORT vp = {};
		vp.MinDepth = min_depth;
		vp.MaxDepth = max_depth;
		vp.Width = w;
		vp.Height = h;
		vp.TopLeftX = x;
		vp.TopLeftY = y;

		list_->RSSetViewports(1, &vp);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetScissor(const D3D12_RECT& rect)
	{
		list_->RSSetScissorRects(1, &rect);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetScissor(UINT left, UINT top, UINT right, UINT bottom)
	{
		D3D12_RECT rect = {};
		rect.left = left;
		rect.right = right;
		rect.top = top;
		rect.bottom = bottom;

		list_->RSSetScissorRects(1, &rect);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetViewportAndScissor(const D3D12_VIEWPORT& vp, const D3D12_RECT& rect)
	{
		SetViewport(vp);
		SetScissor(rect);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetViewportAndScissor(UINT x, UINT y, UINT w, UINT h)
	{
		SetViewport(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
		SetScissor(x, y, x + w, y + h);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetStencilRef(UINT stencil_ref)
	{
		list_->OMSetStencilRef(stencil_ref);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetBlendFactor(float colorRGBA[])
	{
		list_->OMSetBlendFactor(colorRGBA);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
	{
		list_->IASetPrimitiveTopology(topology);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetPipelineState(const GraphicsPSO& pso)
	{
		assert(pso.GetPSO() != nullptr); // check if pso is finalized

		if (graphics_pipeline_state_ != pso.GetPSO())
		{
			graphics_pipeline_state_ = pso.GetPSO();
			list_->SetPipelineState(graphics_pipeline_state_);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetConstants(UINT root_index, UINT num_constants, const void* constants)
	{
		list_->SetGraphicsRoot32BitConstants(root_index, num_constants, constants, 0);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetConstants(UINT root_index, Param32Bit x)
	{
		list_->SetGraphicsRoot32BitConstant(root_index, x.u, 0);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetConstants(UINT root_index, Param32Bit x, Param32Bit y)
	{
		list_->SetGraphicsRoot32BitConstant(root_index, x.u, 0);
		list_->SetGraphicsRoot32BitConstant(root_index, y.u, 1);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetConstants(UINT root_index, Param32Bit x, Param32Bit y, Param32Bit z)
	{
		list_->SetGraphicsRoot32BitConstant(root_index, x.u, 0);
		list_->SetGraphicsRoot32BitConstant(root_index, y.u, 1);
		list_->SetGraphicsRoot32BitConstant(root_index, z.u, 2);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetConstants(UINT root_index, Param32Bit x, Param32Bit y, Param32Bit z, Param32Bit w)
	{
		list_->SetGraphicsRoot32BitConstant(root_index, x.u, 0);
		list_->SetGraphicsRoot32BitConstant(root_index, y.u, 1);
		list_->SetGraphicsRoot32BitConstant(root_index, z.u, 2);
		list_->SetGraphicsRoot32BitConstant(root_index, w.u, 3);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetConstantBuffer(UINT root_index, D3D12_GPU_VIRTUAL_ADDRESS cbv)
	{
		list_->SetGraphicsRootConstantBufferView(root_index, cbv);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetBufferSRV(UINT root_index, GpuBuffer& srv, UINT offset)
	{
		list_->SetGraphicsRootShaderResourceView(root_index, srv->GetGPUVirtualAddress() + offset);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetBufferUAV(UINT root_index, GpuBuffer& uav, UINT offset)
	{
		list_->SetGraphicsRootUnorderedAccessView(root_index, uav->GetGPUVirtualAddress() + offset);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetDescriptorTable(UINT root_index, D3D12_GPU_DESCRIPTOR_HANDLE first_handle)
	{
		list_->SetGraphicsRootDescriptorTable(root_index, first_handle);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& ibv)
	{
		list_->IASetIndexBuffer(&ibv);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetVertexBuffer(UINT slot, const D3D12_VERTEX_BUFFER_VIEW& vbv)
	{
		SetVertexBuffers(slot, 1, &vbv);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::SetVertexBuffers(UINT start_slot, UINT count, const D3D12_VERTEX_BUFFER_VIEW vbvs[])
	{
		list_->IASetVertexBuffers(start_slot, count, vbvs);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::Draw(UINT vertex_count, UINT vertex_start_offset)
	{
		DrawInstanced(vertex_count, 1, vertex_start_offset, 0);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::DrawIndexed(UINT index_count, UINT start_index_location, INT base_vertex_location)
	{
		DrawIndexedInstanced(index_count, 1, start_index_location, base_vertex_location, 0);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::DrawInstanced(UINT vertex_count_per_instance, UINT instance_count, UINT start_vertex_location, UINT start_instance_location)
	{
		FlushResourceBarriers();
		list_->DrawInstanced(vertex_count_per_instance, instance_count, start_vertex_location, start_instance_location);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::DrawIndexedInstanced(UINT index_count_per_instance, UINT instance_count, UINT start_index_location, INT start_vertex_location, UINT start_instance_location)
	{
		FlushResourceBarriers();
		list_->DrawIndexedInstanced(index_count_per_instance, instance_count, start_index_location, start_vertex_location, start_instance_location);
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsContext::DrawIndirect(GpuBuffer& argument_buffer, UINT argument_buffer_offset)
	{
		assert(false); // functionality not implemented yet
	}
}