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

        // Converter for converting wstring to string
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

        name = converter.to_bytes(dxgi_adapter_desc.Description).c_str();
        flags = dxgi_adapter_desc.Flags;
        video_memory = dxgi_adapter_desc.DedicatedVideoMemory;
        system_memory = dxgi_adapter_desc.DedicatedSystemMemory;
        shared_system_memory = dxgi_adapter_desc.SharedSystemMemory;
    }

    //------------------------------------------------------------------------------------------------------
    Vector<Adapter> Adapter::FindAvailableAdapters(FindAdapterFlag find_adapter_flags)
    {
        eastl::vector<Adapter> adapters;

        IDXGIFactory5* factory = nullptr;
        BLOWBOX_ASSERT_HR(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

        IDXGIAdapter1* temp_dxgi_adapter;

        unsigned int i = 0;
        while (factory->EnumAdapters1(i, &temp_dxgi_adapter) != DXGI_ERROR_NOT_FOUND)
        {
            // Convert the temp adapter to an IDXGIAdapter4
            Adapter adapter(temp_dxgi_adapter);

            if ((find_adapter_flags & FindAdapterFlag_NO_INTEL) == FindAdapterFlag_NO_INTEL && (adapter.name[0] == 'I' || adapter.name[1] == 'i'))
            {
                // skip this adapter, it is an Intel adapter
                i++;
                continue;
            }

            if ((find_adapter_flags & FindAdapterFlag_NO_SOFTWARE) == FindAdapterFlag_NO_SOFTWARE && (adapter.flags & DXGI_ADAPTER_FLAG_SOFTWARE) == DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // skip this adapter, it is a software adapter (warp most likely)
                i++;
                continue;
            }

            ID3D12Device* throwaway_device = nullptr;
            if (D3D12CreateDevice(adapter.dxgi_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&throwaway_device)) == S_OK)
            {
                BLOWBOX_RELEASE(throwaway_device);
                adapters.push_back(adapter);
                i++;
            }
            else
            {
                // Current adapter is not DX12 compatible, so skip it.
                BLOWBOX_RELEASE(throwaway_device);
                i++;
                continue;
            }
        }

        if ((find_adapter_flags & FindAdapterFlag_NO_SOFTWARE) != FindAdapterFlag_NO_SOFTWARE)
        {
            IDXGIAdapter4* adapter;
            factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter));

            adapters.push_back(Adapter(adapter));
        }

        BLOWBOX_RELEASE(factory);

        return adapters;
    }
}