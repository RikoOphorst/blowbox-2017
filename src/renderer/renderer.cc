#include "renderer.h"

#include <d3d12.h>
#include <dxgi.h>

#define CHECKHR(hr)

A::A()
{
	IDXGIFactory1* factory_;
	IDXGIAdapter1* adapter_;
	ID3D12Device* device_;

	CHECKHR(CreateDXGIFactory1(IID_PPV_ARGS(&factory_)));

	UINT i = 0;
	while (factory_->EnumAdapters1(i, &adapter_) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter_->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// this adapter is a software adapter, skip it
			i++;
			continue;
		}

		if (D3D12CreateDevice(adapter_, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_)) != S_OK)
		{
			i++;
			continue;
		}
		else
		{
			break;
		}
	}

	if (adapter_ == nullptr)
	{
		//std::cout << "No valid adapter (GPU) was found, creating WARP adapter.." << std::endl;
		//factory_->EnumWarpAdapter(IID_PPV_ARGS(&adapter_));
		//
		//CHECKHR(D3D12CreateDevice(adapter_, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_)));
	}
}