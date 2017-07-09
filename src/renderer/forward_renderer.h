#pragma once

#include "renderer/d3d12_includes.h"
#include "renderer/buffers/depth_buffer.h"

namespace blowbox
{
    class ForwardRenderer
    {
    public:
        ForwardRenderer();
        ~ForwardRenderer();

        void Startup();
        void Render();
    private:
        DepthBuffer depth_buffer_;
    };
}