#pragma once

#include "core/get.h"
#include "win32/window.h"
#include "win32/key_code.h"
#include "renderer/imgui/imgui.h"
#include "renderer/d3d12_includes.h"
#include "renderer/root_signature.h"
#include "renderer/pipeline_state.h"
#include "renderer/buffers/color_buffer.h"
#include "renderer/buffers/upload_buffer.h"

namespace blowbox
{
    class ImGuiManager
    {
    public:
        ImGuiManager();
        ~ImGuiManager();

        void Init();
        void NewFrame();
        void Render();
        void Shutdown();

        static void ImGuiRenderDrawLists(ImDrawData* draw_data);

    protected:
        void CreateDeviceObjects();
        void InvalidateDeviceObjects();

    private:
        bool initialized_;
        INT64 ticks_per_second_;
        INT64 time_;

        ID3DBlob* vertex_shader_blob_;
        D3D12_SHADER_BYTECODE vertex_shader_;

        ID3DBlob* pixel_shader_blob_;
        D3D12_SHADER_BYTECODE pixel_shader_;

        GraphicsPSO pipeline_state_;
        RootSignature root_signature_;
        ColorBuffer texture_buffer_;

        UploadBuffer upload_buffer_;
    };
}