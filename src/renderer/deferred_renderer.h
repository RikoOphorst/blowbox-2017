#pragma once

namespace blowbox
{
    class DeferredRenderer
    {
    public:
        DeferredRenderer();
        ~DeferredRenderer();

        void Startup();
        void Render();
    };
}