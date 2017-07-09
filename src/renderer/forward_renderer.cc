#include "forward_renderer.h"

#include "core/get.h"
#include "renderer/swap_chain.h"
#include "renderer/descriptor_heap.h"
#include "renderer/commands/graphics_context.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    ForwardRenderer::ForwardRenderer()
    {

    }

    //------------------------------------------------------------------------------------------------------
    ForwardRenderer::~ForwardRenderer()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void ForwardRenderer::Startup()
    {
        depth_buffer_.Create(L"DepthBuffer", Get::SwapChain()->GetBufferWidth(), Get::SwapChain()->GetBufferHeight(), DXGI_FORMAT_D32_FLOAT);
    }

    //------------------------------------------------------------------------------------------------------
    void ForwardRenderer::Render()
    {
        GraphicsContext& context = GraphicsContext::Begin(L"SceneRender");

        ID3D12DescriptorHeap* heaps[1] = { Get::CbvSrvUavHeap()->Get() };
        D3D12_DESCRIPTOR_HEAP_TYPE heap_types[1] = { D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV };

        context.SetDescriptorHeaps(1, heap_types, heaps);

        ColorBuffer& back_buffer = Get::SwapChain()->GetBackBuffer();
        context.TransitionResource(back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, true);

        context.SetRenderTarget(back_buffer.GetRTV(), depth_buffer_.GetDSV());
        context.ClearColor(back_buffer);

        context.TransitionResource(depth_buffer_, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
        context.ClearDepth(depth_buffer_);

        context.SetViewportAndScissor(0, 0, Get::SwapChain()->GetBufferWidth(), Get::SwapChain()->GetBufferHeight());

        context.FlushResourceBarriers();

        context.TransitionResource(Get::SwapChain()->GetBackBuffer(), D3D12_RESOURCE_STATE_PRESENT);

        context.Finish(true);

        Get::SwapChain()->Present(false);
    }
}