#include "forward_renderer.h"

#include "core/get.h"
#include "renderer/swap_chain.h"
#include "renderer/descriptor_heap.h"
#include "core/scene/scene_manager.h"
#include "core/debug/performance_profiler.h"
#include "content/file_manager.h"
#include "renderer/material.h"

namespace blowbox
{
    struct PassData
    {
        DirectX::XMFLOAT3 eye_pos;
        float specular_power;
        uint32_t show_diffuse;
        uint32_t show_specular;
        uint32_t show_ambient;
        uint32_t show_emissive;
        DirectX::XMFLOAT4 light_color;
        DirectX::XMFLOAT4 light_position;
        float light_intensity;
        float light_range;
        uint32_t do_normal_mapping;
    };

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

        D3D12_SAMPLER_DESC sampler;
        sampler.BorderColor[0] = 0.0f;
        sampler.BorderColor[1] = 0.0f;
        sampler.BorderColor[2] = 0.0f;
        sampler.BorderColor[3] = 0.0f;
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        sampler.MinLOD = 0.0f;
        sampler.MaxLOD = D3D12_FLOAT32_MAX;
        sampler.MipLODBias = 0.0f;
        sampler.MaxAnisotropy = 8;
        sampler.Filter = D3D12_FILTER_ANISOTROPIC;
        sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

        main_root_signature_.Create(11, 1);
        main_root_signature_[0].InitAsConstantBuffer(0); // Constant buffer per object
        main_root_signature_[1].InitAsConstantBuffer(1); // Constant buffer per material
        main_root_signature_[2].InitAsConstantBuffer(2); // Constant buffer per pass
        main_root_signature_[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // Ambient texture
        main_root_signature_[4].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); // Diffuse texture
        main_root_signature_[5].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); // Emissive texture
        main_root_signature_[6].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // Bump texture
        main_root_signature_[7].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4); // Normal texture
        main_root_signature_[8].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 5); // Shininess texture
        main_root_signature_[9].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 6); // Specular texture
        main_root_signature_[10].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 7); // Opacity texture
        main_root_signature_.InitStaticSampler(0, sampler);
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

        pass_buffer_.Create(L"PassBuffer", 1, sizeof(PassData));
    }

    //------------------------------------------------------------------------------------------------------
    void ForwardRenderer::Render()
    {
        PerformanceProfiler::ProfilerBlock profiler_block("FrameForwardSetup", ProfilerBlockType_RENDERER);

        static float specular_power = 5.0f;
        static DirectX::XMFLOAT4 light_color = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
        static DirectX::XMFLOAT4 light_position = DirectX::XMFLOAT4(7.0f, 14.0f, 20.0f, 1.0f);
        static bool emissive = false;
        static bool ambient = false;
        static bool diffuse = true;
        static bool specular = true;
        static bool normal_mapping = true;
        static float light_intensity = 1.0f;
        static float light_range = 75.0f;

        ImGui::Begin("Stuff");
        ImGui::SliderFloat("Specular Power", &specular_power, 0.1f, 150.0f);
        ImGui::Checkbox("Show Emissive", &emissive);
        ImGui::Checkbox("Show Ambient", &ambient);
        ImGui::Checkbox("Show Diffuse", &diffuse);
        ImGui::Checkbox("Show Specular", &specular);
        ImGui::ColorEdit4("Light Color", &light_color.x, false);
        ImGui::SliderFloat("Light Intensity", &light_intensity, 0.1f, 10.0f);
        ImGui::SliderFloat("Light Range", &light_range, 0.0f, 200.0f);
        ImGui::SliderFloat3("Light Position", &light_position.x, -20.0f, 20.0f);
        ImGui::Checkbox("Do normal mapping", &normal_mapping);
        ImGui::End();

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
                UploadBuffer& object_constant_buffer = entity->GetConstantBuffer();
                SharedPtr<Material> material = entity->GetMaterial().lock();
                UploadBuffer& material_constant_buffer = material->GetConstantBuffer();

                DirectX::XMMATRIX world_transform = entity->GetWorldTransform();

                object_constant_buffer.InsertDataByElement(0, &world_transform);
                object_constant_buffer.InsertDataByElement(1, &(Get::SceneManager()->GetMainCamera()->GetViewMatrix()));
                object_constant_buffer.InsertDataByElement(2, &(Get::SceneManager()->GetMainCamera()->GetProjectionMatrix()));

                PassData pass_data;
                pass_data.eye_pos = Get::SceneManager()->GetMainCamera()->GetPosition();
                pass_data.specular_power = specular_power;
                pass_data.show_emissive = emissive;
                pass_data.show_ambient = ambient;
                pass_data.show_diffuse = diffuse;
                pass_data.show_specular = specular;
                pass_data.light_color = light_color;
                pass_data.light_intensity = light_intensity;
                pass_data.light_range = light_range;
                pass_data.light_position = light_position;
                pass_data.do_normal_mapping = normal_mapping;

                pass_buffer_.InsertDataByElement(0, &pass_data);

                context.SetConstantBuffer(0, object_constant_buffer.GetAddressByElement(0));
                context.SetConstantBuffer(1, material_constant_buffer.GetAddressByElement(0));
                context.SetConstantBuffer(2, pass_buffer_.GetAddressByElement(0));

                BindTexture(context, 3, material->GetTextureAmbient());
                BindTexture(context, 4, material->GetTextureDiffuse());
                BindTexture(context, 5, material->GetTextureEmissive());
                BindTexture(context, 6, material->GetTextureBump());
                BindTexture(context, 7, material->GetTextureNormal());
                BindTexture(context, 8, material->GetTextureShininess());
                BindTexture(context, 9, material->GetTextureSpecular());
                BindTexture(context, 10, material->GetTextureOpacity());

                context.SetVertexBuffer(0, mesh->GetVertexBuffer().GetVertexBufferView());
                context.SetIndexBuffer(mesh->GetIndexBuffer().GetIndexBufferView());

                context.DrawIndexed(mesh->GetMeshData().GetIndices().size());
            }
        }

        context.Finish();
    }

    void ForwardRenderer::BindTexture(GraphicsContext& context, UINT root_signature_slot, WeakPtr<Texture> texture)
    {
        if (!texture.expired())
        {
            UINT srv = texture.lock()->GetBuffer().GetSRV();
            context.SetDescriptorTable(root_signature_slot, Get::CbvSrvUavHeap()->GetGPUDescriptorById(srv));
        }
    }
}