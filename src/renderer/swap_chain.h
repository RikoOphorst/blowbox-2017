#pragma once

#include "util/assert.h"
#include "renderer/buffers/color_buffer.h"

#define SWAP_CHAIN_MAX_BUFFER_COUNT 4

namespace blowbox
{
	class Device;

    /**
    * This class wraps all the functionality of a IDXGISwapChain.
    * It creates the ColorBuffers for the back buffers and makes it
    * easy to swap the buffers via Present calls.
    *
    * @brief Wraps the IDXGISwapChain.
    */
	class SwapChain
	{
	public:
		SwapChain();
		~SwapChain();

        /** 
        * @brief Creates the actual SwapChain.
        * @param[in] output_window The output window to which the SwapChain has to bind.
        * @param[in] width The width of the output window.
        * @param[in] height The height of the output window.
        * @param[in] format The formats you want to use for the back buffers.
        * @param[in] buffer_count The number of back buffers that should be created.
        * @param[in] swap_effect The swap effect that should be used when Presenting to the output window.
        * @param[in] flags Any flags that should be attached to the swap chain.
        * @param[in] disable_alt_enter Whether alt-enter (for fullscreen stuff) should be disabled.
        * @param[in] windowed Whether the swap chain should be windowed or fullscreen.
        */
		void Create(HWND output_window, UINT width, UINT height, DXGI_FORMAT format, UINT buffer_count, DXGI_SWAP_EFFECT swap_effect, UINT flags, bool disable_alt_enter = true, bool windowed = true);
		
        /**
        * @brief Presents the back buffer to the screen.
        * @param[in] vsync Whether the present call should wait for the next V_BLANK. (i.e. whether to use vsync or not)
        */
		void Present(bool vsync = true);

        /** 
        * @brief Retrieve a certain buffer from the SwapChain buffers.
        * @param[in] buffer_index An index into the SwapChain buffers array.
        * @returns A ColorBuffer that represents a SwapChain buffer.
        */
		ColorBuffer& GetBuffer(UINT buffer_index) { BLOWBOX_ASSERT(buffer_index < buffer_count_); return buffers_[buffer_index]; }

        /**
        * @brief Retrieve a certain buffer from the SwapChain buffers.
        * @param[in] buffer_index An index into the SwapChain buffers array.
        * @returns A ColorBuffer that represents a SwapChain buffer.
        */
		const ColorBuffer& GetBuffer(UINT buffer_index) const { BLOWBOX_ASSERT(buffer_index < buffer_count_); return buffers_[buffer_index]; }


        /** @returns A ColorBuffer that represents the current back buffer in the SwapChain. */
		ColorBuffer& GetBackBuffer() { return buffers_[back_buffer_index_]; }

        /** @returns A ColorBuffer that represents the current back buffer in the SwapChain. */
		const ColorBuffer& GetBackBuffer() const { return buffers_[back_buffer_index_]; }

        /** @returns The number of buffers in this SwapChain.*/
		const UINT& GetBufferCount() const { return buffer_count_; }
        /** @returns The number of buffers in this SwapChain.*/
		const UINT& GetBackBufferIndex() const { return back_buffer_index_; }

        /** @returns The width of every buffer in the SwapChain. */
		const UINT& GetBufferWidth() const { return desc_.BufferDesc.Width; }
        /** @returns The height of every buffer in the SwapChain. */
		const UINT& GetBufferHeight() const { return desc_.BufferDesc.Height; }

        /** @brief Implicit cast to a IDXGISwapChain. */
		operator IDXGISwapChain*() { return swap_chain_; }

        /** @brief Overloads the pointer operator to return the underlying IDXGISwapChain. */
		IDXGISwapChain* operator->() { return swap_chain_; }
        /** @brief Overloads the pointer operator to return the underlying IDXGISwapChain. */
		const IDXGISwapChain* operator->() const { return swap_chain_; }
	protected:
        /** 
        * @brief Creates a DXGI_SWAP_CHAIN_DESC based on input params.
        * @param[in] output_window The window that should be output into.
        * @param[in] width The width of the of the SwapChain buffers.
        * @param[in] height The height of the SwapChain buffers.
        * @param[in] format The format for the SwapChain buffers.
        * @param[in] buffer_count The number of buffers in the SwapChain.
        * @param[in] swap_effect The swap effect that should be used when swapping out presenting buffers.
        * @param[in] flags Any flags that should be set on the SwapChain.
        * @param[in] windowed Whether the swap chain should be windowed or not.
        */
		DXGI_SWAP_CHAIN_DESC DescribeSwapChain(HWND output_window, UINT width, UINT height, DXGI_FORMAT format, UINT buffer_count, DXGI_SWAP_EFFECT swap_effect, UINT flags, bool windowed = true);

	private:
		IDXGISwapChain* swap_chain_;                        //!< The underlying IDXGISwapChain.
		DXGI_SWAP_CHAIN_DESC desc_;                         //!< The description of the IDXGISwapChain.
		UINT back_buffer_index_;                            //!< The current back buffer index.
		UINT buffer_count_;                                 //!< The number of buffers in use by the SwapChain.
		ColorBuffer buffers_[SWAP_CHAIN_MAX_BUFFER_COUNT];  //!< All the different SwapChain buffers.
	};
}