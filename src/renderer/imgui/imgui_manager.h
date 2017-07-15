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
    /**
    * This is the custom implementation for ImGui in Blowbox, because all implementations
    * that were online for imgui were quite bad, so I implemented it myself. It wraps all
    * setup, rendering and shutdown for managing ImGui.
    *
    * @brief Implements all ImGui functionality.
    */
    class ImGuiManager
    {
    public:
        /** @brief Constructs the ImGuiManager. */
        ImGuiManager();

        /** @brief Destructs the ImGuiManager. */
        ~ImGuiManager();

        /** @brief Initializes the ImGuiManager. */
        void Init();

        /** @brief Prepares ImGui for a new frame. */
        void NewFrame();

        /** @brief Performs the actual rendering for ImGui. */
        void Render();

        /** @brief Shuts down the ImGuiManager. */
        void Shutdown();

        /**
        * @brief Renders all the ImGui data to the screen.
        * @param[in] draw_data The data that is provided by ImGui.
        */
        static void ImGuiRenderDrawLists(ImDrawData* draw_data);

    protected:
        /** @brief Creates all device objects (shaders, pso, root signature, etc) */
        void CreateDeviceObjects();

        /** @brief Destroys all device objects (shaders, pso, root signature, etc) */
        void InvalidateDeviceObjects();

    private:
        bool initialized_;                      //!< Whether the ImGuiManager has been initialized.
        INT64 ticks_per_second_;                //!< Ticks per second (queried by QueryPerformanceFrequency)
        INT64 time_;                            //!< Current time (queried by QueryPerformanceCounter)

        ID3DBlob* vertex_shader_blob_;          //!< The vertex shader blob for imgui
        D3D12_SHADER_BYTECODE vertex_shader_;   //!< The vertex shader for imgui

        ID3DBlob* pixel_shader_blob_;           //!< The pixel shader blob for imgui
        D3D12_SHADER_BYTECODE pixel_shader_;    //!< The pixel shader for imgui

        GraphicsPSO pipeline_state_;            //!< The PSO for imgui rendering
        RootSignature root_signature_;          //!< The rootsignature for imgui rendering
        ColorBuffer texture_buffer_;            //!< The buffer for all texture data provided by imgui

        UploadBuffer upload_buffer_;            //!< One large buffer for storing all vertex, index, constant buffer, etc. data
    };
}