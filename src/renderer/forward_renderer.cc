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
		eastl::shared_ptr<SwapChain> swap_chain = Get::SwapChain();
        depth_buffer_.Create(L"DepthBuffer", swap_chain->GetBufferWidth(), swap_chain->GetBufferHeight(), DXGI_FORMAT_D32_FLOAT);
    }

    //------------------------------------------------------------------------------------------------------
    void ForwardRenderer::Render()
    {
        GraphicsContext& context = GraphicsContext::Begin(L"CommandListForwardRendering");

		eastl::shared_ptr<SwapChain> swap_chain = Get::SwapChain();

        ColorBuffer& back_buffer = swap_chain->GetBackBuffer();
        context.SetRenderTarget(back_buffer.GetRTV(), depth_buffer_.GetDSV());
        context.ClearColor(back_buffer);

        context.TransitionResource(depth_buffer_, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
        context.ClearDepth(depth_buffer_);

        context.SetViewportAndScissor(0, 0, swap_chain->GetBufferWidth(), swap_chain->GetBufferHeight());

        context.Finish();
    }
}