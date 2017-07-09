#include "device.h"

#include "core/get.h"
#include <codecvt>
#include <locale>

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	Device::Device() :
		device_(nullptr)
	{

	}
	
	//------------------------------------------------------------------------------------------------------
	Device::~Device()
	{
		BLOWBOX_RELEASE(device_);
	}
	
	//------------------------------------------------------------------------------------------------------
	void Device::Create()
	{
#if defined(DEBUG) || defined(_DEBUG)
		{
			ID3D12Debug* debug_controller;
			BLOWBOX_ASSERT_HR(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)));
			debug_controller->EnableDebugLayer();
			debug_controller->Release();
		}
#endif

        // As the Adapter, pass in a nullptr. This makes D3D12 look for the first available adapter.
        BLOWBOX_ASSERT_HR(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_)));
	}
	
	//------------------------------------------------------------------------------------------------------
	void Device::Create(const Adapter& adapter)
	{
#ifdef _DEBUG
        ID3D12Debug* debug_controller;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
        {
            debug_controller->EnableDebugLayer();
        }
        debug_controller->Release();
#endif

        BLOWBOX_ASSERT_HR(D3D12CreateDevice(adapter.dxgi_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_)));
	}

    //------------------------------------------------------------------------------------------------------
    const Adapter& Device::GetAdapter() const
    {
        return adapter_;
    }

    //------------------------------------------------------------------------------------------------------
    ID3D12Device* Device::Get() const
    {
        return device_;
    }
    
    //------------------------------------------------------------------------------------------------------
    eastl::vector<Adapter> Device::FindAvailableAdapters(FindAdapterFlag find_adapter_flags)
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