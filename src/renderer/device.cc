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

        device_->SetStablePowerState(true);
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

        adapter_ = adapter;
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
}