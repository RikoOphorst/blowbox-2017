#include "imgui_manager.h"

#include "core/debug/performance_profiler.h"
#include "renderer/swap_chain.h"
#include "renderer/descriptor_heap.h"
#include "renderer/commands/command_context.h"
#include "renderer/commands/graphics_context.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    ImGuiManager::ImGuiManager() : 
        initialized_(false),
        vertex_shader_blob_(nullptr),
        pixel_shader_blob_(nullptr)
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    ImGuiManager::~ImGuiManager()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    void ImGuiManager::Init()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeyMap[ImGuiKey_Tab] = KeyCode_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = KeyCode_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = KeyCode_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = KeyCode_UP;
        io.KeyMap[ImGuiKey_DownArrow] = KeyCode_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = KeyCode_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = KeyCode_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = KeyCode_HOME;
        io.KeyMap[ImGuiKey_End] = KeyCode_END;
        io.KeyMap[ImGuiKey_Delete] = KeyCode_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = KeyCode_BACKSPACE;
        io.KeyMap[ImGuiKey_Enter] = KeyCode_ENTER;
        io.KeyMap[ImGuiKey_Escape] = KeyCode_ESCAPE;
        io.KeyMap[ImGuiKey_A] = KeyCode_A;
        io.KeyMap[ImGuiKey_C] = KeyCode_C;
        io.KeyMap[ImGuiKey_V] = KeyCode_V;
        io.KeyMap[ImGuiKey_X] = KeyCode_X;
        io.KeyMap[ImGuiKey_Y] = KeyCode_Y;
        io.KeyMap[ImGuiKey_Z] = KeyCode_Z;

        io.RenderDrawListsFn = ImGuiRenderDrawLists;
        io.ImeWindowHandle = Get::MainWindow()->GetWindowHandle();

        QueryPerformanceFrequency((LARGE_INTEGER*)&ticks_per_second_);
        QueryPerformanceCounter((LARGE_INTEGER*)&time_);
    }
    
    //------------------------------------------------------------------------------------------------------
    void ImGuiManager::NewFrame()
    {
        if (!initialized_)
        {
            CreateDeviceObjects();
        }

        ImGuiIO& io = ImGui::GetIO();

		eastl::shared_ptr<Window> main_window = Get::MainWindow();

        io.DisplaySize = ImVec2(
            static_cast<float>(main_window->GetWindowResolution().width), 
            static_cast<float>(main_window->GetWindowResolution().height)
        );

        INT64 current_time;
        QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
        io.DeltaTime = (float)(current_time - time_) / ticks_per_second_;
        time_ = current_time;

        io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
        io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
        io.KeySuper = false;

        KeyboardState& keyboard_state = main_window->GetKeyboardState();
        MouseState& mouse_state = main_window->GetMouseState();

        for (auto it = keyboard_state.key_states_.begin(); it != keyboard_state.key_states_.end(); it++)
        {
            io.KeysDown[it->first] = it->second.down;
        }

        io.MouseDown[0] = mouse_state.GetButton(MouseButton_LEFT);
        io.MouseDown[1] = mouse_state.GetButton(MouseButton_MIDDLE);
        io.MouseDown[2] = mouse_state.GetButton(MouseButton_RIGHT);

        Queue<unsigned int> input_keys = keyboard_state.GetInputKeys();

        while (!input_keys.empty())
        {
            unsigned int current = input_keys.front();

            if (current > 0 && current < 0x10000)
            {
                io.AddInputCharacter(static_cast<unsigned short>(input_keys.front()));
            }

            input_keys.pop();
        }

        io.MousePos = ImVec2(mouse_state.GetMousePosition().x, mouse_state.GetMousePosition().y);

        io.MouseWheel = mouse_state.GetScrollDelta().y;

        ImGui::NewFrame();
    }
    
    //------------------------------------------------------------------------------------------------------
    void ImGuiManager::Render()
    {
        PerformanceProfiler::ProfilerBlock block("RenderImGui", ProfilerBlockType_RENDERER);
        ImGui::Render();
    }
    
    //------------------------------------------------------------------------------------------------------
    void ImGuiManager::Shutdown()
    {
        InvalidateDeviceObjects();
        ImGui::Shutdown();
    }
    
    //------------------------------------------------------------------------------------------------------
    void ImGuiManager::ImGuiRenderDrawLists(ImDrawData* draw_data)
    {
        eastl::shared_ptr<ImGuiManager> imgui_manager = Get::ImGuiManager();
        
        BYTE* mapped_data = imgui_manager->upload_buffer_.GetMappedData();
        
        BYTE* write_cursor = mapped_data;
        
        // Copy the projection matrix at the beginning of the buffer
        {
            float translate = -0.5f * 2.f;
            const float L = 0.f;
            const float R = ImGui::GetIO().DisplaySize.x;
            const float B = ImGui::GetIO().DisplaySize.y;
            const float T = 0.f;
            const float mvp[4][4] =
            {
                { 2.0f / (R - L),       0.0f,                   0.0f,       0.0f },
                { 0.0f,                 2.0f / (T - B),         0.0f,       0.0f },
                { 0.0f,                 0.0f,                   0.5f,       0.0f },
                { (R + L) / (L - R),    (T + B) / (B - T),      0.5f,       1.0f },
            };
        
            memcpy(write_cursor, &mvp[0], sizeof(mvp));
            write_cursor += sizeof(mvp);
        }
        
        // Copy the vertices and indices for each command list
        for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];
            size_t vertices_count = cmd_list->VtxBuffer.size();
            size_t indices_count = cmd_list->IdxBuffer.size();
            size_t vertices_size = vertices_count * sizeof(ImDrawVert);
            size_t indices_size = indices_count * sizeof(ImDrawIdx);
        
            // Copy the vertex data
            memcpy(write_cursor, &cmd_list->VtxBuffer[0], vertices_size);
            write_cursor += vertices_size;
        
            // Copy the index data
            memcpy(write_cursor, &cmd_list->IdxBuffer[0], indices_size);
            write_cursor += indices_size;
        }
        
        GraphicsContext& context = GraphicsContext::Begin(L"CommandListImGui");
        
        D3D12_VIEWPORT viewport;
        viewport.Width = ImGui::GetIO().DisplaySize.x;
        viewport.Height = ImGui::GetIO().DisplaySize.y;
        viewport.MinDepth = 0.f;
        viewport.MaxDepth = 1.f;
        viewport.TopLeftX = 0.f;
        viewport.TopLeftY = 0.f;
        
        context.SetViewport(viewport);
        context.SetRenderTarget(Get::SwapChain()->GetBackBuffer().GetRTV());
        context.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context.SetPipelineState(imgui_manager->pipeline_state_);
        context.SetRootSignature(imgui_manager->root_signature_);
        
        D3D12_GPU_VIRTUAL_ADDRESS buffer_address = imgui_manager->upload_buffer_.GetAddressByElement(0);
        context.SetConstantBuffer(1, buffer_address);
        context.SetDescriptorTable(0, Get::CbvSrvUavHeap()->GetGPUDescriptorById(imgui_manager->texture_buffer_.GetSRV()));
        
        int64_t read_cursor = 64; // start at 64 because the constant buffer is 64 bytes in size - one mat44
        
        for (int i = 0; i < draw_data->CmdListsCount; i++)
        {
            // Render command lists
            int vtx_offset = 0;
            int idx_offset = 0;
        
            const ImDrawList* cmd_list = draw_data->CmdLists[i];
            size_t vertices_count = cmd_list->VtxBuffer.size();
            size_t indices_count = cmd_list->IdxBuffer.size();
            size_t vertices_size = vertices_count * sizeof(ImDrawVert);
            size_t indices_size = indices_count * sizeof(ImDrawIdx);
        
            D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
            vertex_buffer_view.BufferLocation = buffer_address + read_cursor;
            vertex_buffer_view.StrideInBytes = sizeof(ImDrawVert);
            vertex_buffer_view.SizeInBytes = vertices_size;
            read_cursor += vertices_size;
        
            D3D12_INDEX_BUFFER_VIEW index_buffer_view;
            index_buffer_view.BufferLocation = buffer_address + read_cursor;
            index_buffer_view.SizeInBytes = indices_size;
            index_buffer_view.Format = DXGI_FORMAT_R16_UINT;
            read_cursor += indices_size;
        
            context.SetVertexBuffers(0, 1, &vertex_buffer_view);
            context.SetIndexBuffer(index_buffer_view);
        
            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
            {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                if (pcmd->UserCallback)
                {
                    pcmd->UserCallback(cmd_list, pcmd);
                }
                else
                {
                    const D3D12_RECT scissor_rect = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
        
                    context.SetScissor(scissor_rect);
                    context.DrawIndexedInstanced(pcmd->ElemCount, 1, idx_offset, vtx_offset, 0);
                }
        
                idx_offset += pcmd->ElemCount;
            }
        
            vtx_offset += vertices_count;
        }
        
        context.Finish();
    }
    
    //------------------------------------------------------------------------------------------------------
    void ImGuiManager::CreateDeviceObjects()
    {
        // Create the Vertex Shader
        {
            static const char* vertex_shader =
                "cbuffer vertexBuffer : register(b0) \
                {\
                    float4x4 ProjectionMatrix; \
                };\
                \
                struct VS_INPUT\
                {\
                    float2 pos : POSITION;\
                    float4 col : COLOR0;\
                    float2 uv  : TEXCOORD0;\
                };\
                \
                struct PS_INPUT\
                {\
                    float4 pos : SV_POSITION;\
                    float4 col : COLOR0;\
                    float2 uv  : TEXCOORD0;\
                };\
                \
                PS_INPUT main(VS_INPUT input)\
                {\
                    PS_INPUT output;\
                    output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
                    output.col = input.col;\
                    output.uv  = input.uv;\
                    return output;\
                }";
        
            D3DCompile(vertex_shader, strlen(vertex_shader), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &vertex_shader_blob_, NULL);
            BLOWBOX_ASSERT(vertex_shader_blob_ != nullptr);
        
            vertex_shader_.pShaderBytecode = vertex_shader_blob_->GetBufferPointer();
            vertex_shader_.BytecodeLength = vertex_shader_blob_->GetBufferSize();
        }
        
        // Create the Pixel Shader
        {
            static const char* pixel_shader =
                "struct PS_INPUT\
                {\
                    float4 pos : SV_POSITION;\
                    float4 col : COLOR0;\
                    float2 uv  : TEXCOORD0;\
                };\
                \
                SamplerState sampler0 : register(s0);\
                Texture2D texture0 : register(t0);\
                \
                float4 main(PS_INPUT input) : SV_Target\
                {\
                    float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
                    return out_col; \
                }";
        
            D3DCompile(pixel_shader, strlen(pixel_shader), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pixel_shader_blob_, NULL);
            BLOWBOX_ASSERT(pixel_shader_blob_ != nullptr);
        
            pixel_shader_.pShaderBytecode = pixel_shader_blob_->GetBufferPointer();
            pixel_shader_.BytecodeLength = pixel_shader_blob_->GetBufferSize();
        }
        
        D3D12_INPUT_ELEMENT_DESC input_element_descs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, (size_t)(&((ImDrawVert*)0)->pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, (size_t)(&((ImDrawVert*)0)->uv),  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, (size_t)(&((ImDrawVert*)0)->col), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };
        
        D3D12_RASTERIZER_DESC rasterizer_desc; 
        rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizer_desc.CullMode = D3D12_CULL_MODE_NONE;
        rasterizer_desc.FrontCounterClockwise = TRUE;
        rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterizer_desc.DepthClipEnable = TRUE;
        rasterizer_desc.MultisampleEnable = FALSE;
        rasterizer_desc.AntialiasedLineEnable = TRUE;
        rasterizer_desc.ForcedSampleCount = 1;
        rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        
        D3D12_BLEND_DESC blend_desc;
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].LogicOpEnable = FALSE;
        blend_desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        
        D3D12_DEPTH_STENCIL_DESC depth_stencil_desc; 
        depth_stencil_desc.DepthEnable = FALSE;
        depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
        depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        depth_stencil_desc.StencilEnable = FALSE;
        depth_stencil_desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        depth_stencil_desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        depth_stencil_desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        depth_stencil_desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        depth_stencil_desc.BackFace = depth_stencil_desc.FrontFace;
        
        D3D12_SAMPLER_DESC sampler_desc;
        sampler_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; 
        sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.MipLODBias = 0.f;
        sampler_desc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        sampler_desc.MinLOD = 0.f;
        sampler_desc.MaxLOD = 0.f;
        sampler_desc.MaxAnisotropy = 8;
        
        root_signature_.Create(2, 1);
        root_signature_[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
        root_signature_[1].InitAsConstantBuffer(0);
        root_signature_.InitStaticSampler(0, sampler_desc);
        root_signature_.Finalize(L"RootSignatureImGui", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
        
        pipeline_state_.SetVertexShader(vertex_shader_);
        pipeline_state_.SetPixelShader(pixel_shader_);
        pipeline_state_.SetRasterizerState(rasterizer_desc);
        pipeline_state_.SetBlendState(blend_desc);
        pipeline_state_.SetDepthStencilState(depth_stencil_desc);
        pipeline_state_.SetInputLayout(3, input_element_descs);
        pipeline_state_.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
        pipeline_state_.SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
        pipeline_state_.SetSampleMask(UINT_MAX);
        pipeline_state_.SetRootSignature(root_signature_);
        pipeline_state_.Finalize();
        
        unsigned char* pixels = 0;
        int width, height;
        
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        
        texture_buffer_.Create(L"ImGuiFontBuffer", width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
        
        D3D12_SUBRESOURCE_DATA resource_data = {};
        resource_data.pData = pixels;
        resource_data.RowPitch = width * 4;
        resource_data.SlicePitch = width * 4 * height;
        CommandContext::InitializeTexture(texture_buffer_, 1, &resource_data);
        
        io.Fonts->TexID = texture_buffer_;
        
        upload_buffer_.Create(L"ImGuiUploadBuffer", 1, 1024 * 1024 * 8, nullptr);
        
        initialized_ = true;
    }
    
    //------------------------------------------------------------------------------------------------------
    void ImGuiManager::InvalidateDeviceObjects()
    {
        BLOWBOX_RELEASE(vertex_shader_blob_);
        BLOWBOX_RELEASE(pixel_shader_blob_);
    }
}