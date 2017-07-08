#pragma once

#include "util/assert.h"
#include "renderer/buffers/color_buffer.h"

#define SWAP_CHAIN_MAX_BUFFER_COUNT 3

namespace blowbox
{
	class Device;

	class SwapChain
	{
	public:
		SwapChain();
		~SwapChain();

		void Create(HWND output_window, UINT width, UINT height, DXGI_FORMAT format, UINT buffer_count, DXGI_SWAP_EFFECT swap_effect, UINT flags, bool disable_alt_enter = true, bool windowed = true);
		
		void Present(bool vsync = true);

		ColorBuffer& GetBuffer(UINT buffer_index) { BLOWBOX_ASSERT(buffer_index < buffer_count_); return buffers_[buffer_index]; }
		const ColorBuffer& GetBuffer(UINT buffer_index) const { BLOWBOX_ASSERT(buffer_index < buffer_count_); return buffers_[buffer_index]; }

		ColorBuffer& GetBackBuffer() { return buffers_[back_buffer_index_]; }
		const ColorBuffer& GetBackBuffer() const { return buffers_[back_buffer_index_]; }

		const UINT& GetBufferCount() const { return buffer_count_; }
		const UINT& GetBackBufferIndex() const { return back_buffer_index_; }

		const UINT& GetBufferWidth() const { return desc_.BufferDesc.Width; }
		const UINT& GetBufferHeight() const { return desc_.BufferDesc.Height; }

		operator IDXGISwapChain*() { return swap_chain_; }

		IDXGISwapChain* operator->() { return swap_chain_; }
		const IDXGISwapChain* operator->() const { return swap_chain_; }
	protected:
		DXGI_SWAP_CHAIN_DESC DescribeSwapChain(HWND output_window, UINT width, UINT height, DXGI_FORMAT format, UINT buffer_count, DXGI_SWAP_EFFECT swap_effect, UINT flags, bool windowed = true);

	private:
		IDXGISwapChain* swap_chain_;
		DXGI_SWAP_CHAIN_DESC desc_;
		UINT back_buffer_index_;
		UINT buffer_count_;
		ColorBuffer buffers_[SWAP_CHAIN_MAX_BUFFER_COUNT];
	};
}