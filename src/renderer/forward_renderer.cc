#include "forward_renderer.h"

#include "core/get.h"
#include "renderer/swap_chain.h"
#include "renderer/descriptor_heap.h"
#include "renderer/commands/graphics_context.h"
#include "core/scene/scene_manager.h"
#include "core/debug/performance_profiler.h"
#include "content/file_manager.h"

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
		SharedPtr<SwapChain> swap_chain = Get::SwapChain();
        depth_buffer_.Create(L"DepthBuffer", swap_chain->GetBufferWidth(), swap_chain->GetBufferHeight(), DXGI_FORMAT_D32_FLOAT);

        vertex_shader_.Create(Get::FileManager()->GetTextFile("./shaders/vertex.hlsl"), ShaderType_VERTEX);
        pixel_shader_.Create(Get::FileManager()->GetTextFile("./shaders/pixel.hlsl"), ShaderType_PIXEL);

        main_root_signature_.Create(1, 0);
        main_root_signature_[0].InitAsConstantBuffer(0); // Constant buffer per object
        main_root_signature_.Finalize(L"RootSignatureForwardRendering", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        D3D12_BLEND_DESC blend_state = {};
        blend_state.IndependentBlendEnable = FALSE;
        blend_state.RenderTarget[0].BlendEnable = TRUE;
        blend_state.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
        blend_state.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        blend_state.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
        blend_state.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
        blend_state.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
        blend_state.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        blend_state.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        D3D12_RASTERIZER_DESC rasterizer_state = {};
        rasterizer_state.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizer_state.CullMode = D3D12_CULL_MODE_NONE;
        rasterizer_state.FrontCounterClockwise = TRUE;
        rasterizer_state.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterizer_state.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterizer_state.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterizer_state.DepthClipEnable = TRUE;
        rasterizer_state.MultisampleEnable = FALSE;
        rasterizer_state.AntialiasedLineEnable = FALSE;
        rasterizer_state.ForcedSampleCount = 0;
        rasterizer_state.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        D3D12_DEPTH_STENCIL_DESC depth_stencil_state = {};
        depth_stencil_state.DepthEnable = TRUE;
        depth_stencil_state.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        depth_stencil_state.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        depth_stencil_state.StencilEnable = FALSE;
        depth_stencil_state.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        depth_stencil_state.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

        main_pso_.SetBlendState(blend_state);
        main_pso_.SetRasterizerState(rasterizer_state);
        main_pso_.SetDepthStencilState(depth_stencil_state);
        main_pso_.SetInputLayout(Vertex::GetInputElements().size(), &(Vertex::GetInputElements()[0]));
        main_pso_.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
        main_pso_.SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
        main_pso_.SetRootSignature(main_root_signature_);
        main_pso_.SetVertexShader(vertex_shader_.GetShaderByteCode());
        main_pso_.SetPixelShader(pixel_shader_.GetShaderByteCode());
        main_pso_.SetSampleMask(0xFFFFFFFF);
        main_pso_.Finalize();
    }

    //------------------------------------------------------------------------------------------------------
    void ForwardRenderer::Render()
    {
        PerformanceProfiler::ProfilerBlock profiler_block("FrameForwardSetup", ProfilerBlockType_RENDERER);

        GraphicsContext& context = GraphicsContext::Begin(L"CommandListForwardSetup");

		SharedPtr<SwapChain> swap_chain = Get::SwapChain();

        ColorBuffer& back_buffer = swap_chain->GetBackBuffer();
        context.SetRenderTarget(back_buffer.GetRTV(), depth_buffer_.GetDSV());
        context.ClearColor(back_buffer);

        context.TransitionResource(depth_buffer_, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
        context.ClearDepth(depth_buffer_);

        context.SetViewportAndScissor(0, 0, swap_chain->GetBufferWidth(), swap_chain->GetBufferHeight());

        profiler_block.Finish();
        PerformanceProfiler::ProfilerBlock profiler_block2("FrameRecordingDrawCalls", ProfilerBlockType_RENDERER);

        Vector<SharedPtr<Entity>>& entities = Get::SceneManager()->GetEntities();

        context.SetPipelineState(main_pso_);
        context.SetRootSignature(main_root_signature_);

        for (int i = 0; i < entities.size(); i++)
        {
            SharedPtr<Entity>& entity = entities[i];
            SharedPtr<Mesh> mesh = entity->GetMesh();

            if (mesh != nullptr)
            {
                context.SetPrimitiveTopology(mesh->GetMeshData().GetTopology());
                UploadBuffer& constant_buffer = entity->GetConstantBuffer();

                DirectX::XMMATRIX world_transform = entity->GetWorldTransform();

                constant_buffer.InsertDataByElement(0, &world_transform);
                constant_buffer.InsertDataByElement(1, &(Get::SceneManager()->GetMainCamera()->GetViewMatrix()));
                constant_buffer.InsertDataByElement(2, &(Get::SceneManager()->GetMainCamera()->GetProjectionMatrix()));

                context.SetConstantBuffer(0, constant_buffer.GetAddressByElement(0));

                context.SetVertexBuffer(0, mesh->GetVertexBuffer().GetVertexBufferView());
                context.SetIndexBuffer(mesh->GetIndexBuffer().GetIndexBufferView());

                context.DrawIndexed(mesh->GetMeshData().GetIndices().size());
            }
        }

        context.Finish();
    }
}