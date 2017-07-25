#pragma once

#include "renderer/d3d12_includes.h"
#include "renderer/buffers/depth_buffer.h"
#include "renderer/buffers/structured_buffer.h"
#include "renderer/pipeline_state.h"
#include "renderer/root_signature.h"
#include "renderer/commands/graphics_context.h"
#include "renderer/shader.h"
#include "util/weak_ptr.h"

#define BLOWBOX_MAX_LIGHTS_PER_TYPE 128

namespace blowbox
{
    class Texture;

    /**
    * The ForwardRenderer renders the entire scene in a forward style, often
    * it is called the "painters" algorithm, because everything is kept on
    * being overlaid on top of each other.
    *
    * @brief Implements a forward rendering technique.
    */
    class ForwardRenderer
    {
    public:
        /** @brief Constructs the ForwardRenderer. */
        ForwardRenderer();

        /** @brief Destructs the ForwardRenderer. */
        ~ForwardRenderer();

        /** @brief Starts up the ForwardRenderer. */
        void Startup();

        /** @brief Renders the entire scene into the current backbuffer in the SwapChain. */
        void Render();

    protected:
        void BindTexture(GraphicsContext& context, UINT root_signature_slot, WeakPtr<Texture> texture);

        void PrepareRenderTargets();
        void PreparePassBuffer();
        void PrepareDirectionalLights();
        void PreparePointLights();
        void PrepareSpotLights();

    private:
        Shader vertex_shader_;                          //!< Vertex shader for the forward rendering.
        Shader pixel_shader_;                           //!< Pixel shader for the forward rendering.
        RootSignature main_root_signature_;             //!< The main root signature for all forward rendering.
        GraphicsPSO main_pso_;                          //!< The main PSO that is used for all forward rendering.
        DepthBuffer depth_buffer_;                      //!< The depth buffer that is used to render the scene.
        UploadBuffer pass_buffer_;                      //!< Buffer for storing pass data.
        StructuredBuffer directional_lights_buffer_;    //!< Buffer for storing all directional lights.
        StructuredBuffer point_lights_buffer_;          //!< Buffer for storing all point lights.
        StructuredBuffer spot_lights_buffer_;           //!< Buffer for storing all spot lights.
    };
}