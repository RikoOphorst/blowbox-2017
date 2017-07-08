#include "swap_chain.h"

#include "renderer.h"
#include "device.h"
#include "renderer/commands/command_manager.h"
#include "renderer/commands/command_queue.h"
#include "core/get.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	SwapChain::SwapChain() :
		swap_chain_(nullptr),
		back_buffer_index_(0),
		buffer_count_(0)
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	SwapChain::~SwapChain()
	{
		BLOWBOX_RELEASE(swap_chain_);
	}

	//------------------------------------------------------------------------------------------------------
	void SwapChain::Create(HWND output_window, UINT width, UINT height, DXGI_FORMAT format, UINT buffer_count, DXGI_SWAP_EFFECT swap_effect, UINT flags, bool disable_alt_enter, bool windowed)
	{
		assert(buffer_count <= SWAP_CHAIN_MAX_BUFFER_COUNT);

		DXGI_SWAP_CHAIN_DESC desc = DescribeSwapChain(output_window, width, height, format, buffer_count, swap_effect, flags, windowed);

        IDXGIFactory5* factory;
        BLOWBOX_ASSERT_HR(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

		BLOWBOX_ASSERT_HR(factory->CreateSwapChain(Get::CommandManager()->GetGraphicsQueue()->Get(), &desc, &swap_chain_));

		if (disable_alt_enter)
		{
			IDXGIFactory* factory;
			swap_chain_->GetParent(IID_PPV_ARGS(&factory));
			BLOWBOX_ASSERT_HR(factory->MakeWindowAssociation(output_window, DXGI_MWA_NO_ALT_ENTER));
		}

		for (UINT i = 0; i < buffer_count; i++)
		{
			ID3D12Resource* temp_buf = nullptr;
			BLOWBOX_ASSERT_HR(swap_chain_->GetBuffer(i, IID_PPV_ARGS(&temp_buf)));
			
			float bbc[4] = { 0.1f, 0.1f, 0.4f, 1.0f };
			buffers_[i] = ColorBuffer(bbc);
			buffers_[i].CreateFromSwapChain(L"SwapChainBuffer", temp_buf);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void SwapChain::Present(bool vsync)
	{
		swap_chain_->Present(vsync ? 1 : 0, 0);
		back_buffer_index_ = (back_buffer_index_ + 1) % buffer_count_;
	}
	
	//------------------------------------------------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC SwapChain::DescribeSwapChain(HWND output_window, UINT width, UINT height, DXGI_FORMAT format, UINT buffer_count, DXGI_SWAP_EFFECT swap_effect, UINT flags, bool windowed)
	{
		desc_ = {};
		desc_.BufferDesc.Width = width;
		desc_.BufferDesc.Height = height;
		desc_.BufferDesc.RefreshRate.Numerator = 60;
		desc_.BufferDesc.RefreshRate.Denominator = 1;
		desc_.BufferDesc.Format = format;
		desc_.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc_.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc_.SampleDesc.Count = 1;
		desc_.SampleDesc.Quality = 0;
		desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc_.BufferCount = buffer_count;
		desc_.OutputWindow = output_window;
		desc_.Windowed = windowed;
		desc_.SwapEffect = swap_effect;
		desc_.Flags = flags;

		buffer_count_ = buffer_count;

		return desc_;
	}
}