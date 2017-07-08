#pragma once

#include "renderer/d3d12_includes.h"
#include "EASTL/string.h"

namespace blowbox
{
    struct Adapter
    {
        Adapter();
        Adapter(IDXGIAdapter1* dxgi_adapter);

        IDXGIAdapter4* dxgi_adapter;
        eastl::string name;
        DXGI_ADAPTER_FLAG3 flags;
        unsigned long long video_memory;
        unsigned long long system_memory;
        unsigned long long shared_system_memory;
        LUID adapter_luid;
    };
}