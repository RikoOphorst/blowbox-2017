#include "adapter.h"

#include <locale>
#include <codecvt>

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Adapter::Adapter() :
        dxgi_adapter(nullptr),
        name("")
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    Adapter::Adapter(IDXGIAdapter1* _dxgi_adapter) :
        dxgi_adapter(reinterpret_cast<IDXGIAdapter4*>(_dxgi_adapter))
    {
        // Extract the adapter description (for adapter info such as vendor, vram, etc)
        DXGI_ADAPTER_DESC3 dxgi_adapter_desc;
        dxgi_adapter->GetDesc3(&dxgi_adapter_desc);

        // Convert the adapter description from a wstring to a string
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;

        name = converter.to_bytes(dxgi_adapter_desc.Description).c_str();
        flags = dxgi_adapter_desc.Flags;
        video_memory = dxgi_adapter_desc.DedicatedVideoMemory;
        system_memory = dxgi_adapter_desc.DedicatedSystemMemory;
        shared_system_memory = dxgi_adapter_desc.SharedSystemMemory;
    }
}