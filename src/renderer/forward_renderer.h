#pragma once

#include "renderer/d3d12_includes.h"
#include "renderer/buffers/depth_buffer.h"

namespace blowbox
{
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
    private:
        DepthBuffer depth_buffer_; //!< The depth buffer that is used to render the scene.
    };
}