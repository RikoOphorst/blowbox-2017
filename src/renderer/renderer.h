#pragma once

#include "renderer/d3d12_includes.h"
#include "renderer/device.h"
#include "renderer/swap_chain.h"
#include "renderer/descriptor_heap.h"
#include "renderer/buffers/buffer_manager.h"
#include "renderer/buffers/upload_buffer.h"
#include "renderer/buffers/depth_buffer.h"

namespace blowbox
{
	class Window;
	class RootSignature;
	class Shader;
	class DepthStencil;
	class CommandContext;
	class GraphicsContext;
	
	/**
	* @class blowbox::Renderer
	* @author Riko Ophorst
	* @brief Forward DirectX 12 renderer
	*/
	class Renderer
	{
	public:
		/**
		* @brief Renderer constructor
		*/
		Renderer();

		/**
		* @brief Renderer destructor
		*/
		~Renderer();

		/**
		* @brief Starts up the renderer, initializes everything, etc.
		*/
		void Startup();

		/**
		* @brief Makes the renderer draw to screen
		*/
		void Draw();
		
        const Device& GetDevice() const { return device_; }

		DescriptorHeap& GetRtvHeap() { return rtv_heap_; }
		DescriptorHeap& GetDsvHeap() { return dsv_heap_; }
		DescriptorHeap& GetCbvSrvUavHeap() { return cbv_srv_uav_heap_; }
		DescriptorHeap& GetSamplerHeap() { return sampler_heap_; }

		SwapChain& GetSwapChain() { return swap_chain_; }

		inline BufferManager& GetBufferManager() { return buffer_manager_; }

		UploadBuffer& GetPassConstantsBuffer() { return pass_constants_; }
		UploadBuffer& GetDebugConstantsBuffer() { return debug_constants_; }

		UploadBuffer& GetMaterialConstantsBuffer() { return material_constants_; }
		UINT GetNumMaterialCbs() { return num_material_cbs_; }
		void SetNumMaterialCbs(UINT num_material_cbs) { num_material_cbs_ = num_material_cbs; }
		UINT GetNewMaterialConstantBufferID() { return num_material_cbs_++; }

		UploadBuffer& GetObjectConstantsBuffer() { return object_constants_; }
		UINT GetNumObjectCbs() { return num_object_cbs_; }
		void SetNumObjectCbs(UINT num_object_cbs) { num_object_cbs_ = num_object_cbs; }
		UINT GetNewObjectConstantBufferID() { return num_object_cbs_++; }

	protected:
		void CreateDevice(); //!< Creates the Direct3D device
		void CreateCommandObjects(); //!< Creates the command objects (queue, allocator, list)
		void CreateSwapChain(); //!< Creates the swap chain
		void CreateDescriptorHeaps(); //!< Creates the resource heaps for the render target views & depth/stencil views
		void CreatePSOs(); //!< Creates the default pipeline state object
		void CreateBuffers(); //!< Creates all the buffered frame resources
		void FlushCommandQueue(); //!< Informs the renderer to flush the entire command queue

		void CheckDeviceRemoved(); //!< Check whether the ID3D12Device was removed and for what reason (outputs to console)

	private:
		Device device_;
		SwapChain swap_chain_;
		DepthBuffer depth_buffer_;

		DescriptorHeap rtv_heap_; //!< Heap to store descriptors of the render target views
		DescriptorHeap dsv_heap_; //!< Heap to store descriptors of the depth/stencil views
		DescriptorHeap cbv_srv_uav_heap_; //!< Heap to store descriptors of shader resource views
		DescriptorHeap sampler_heap_; //!< Heap to store descriptors of sampler states

		UploadBuffer pass_constants_;
		UploadBuffer debug_constants_;
		UploadBuffer material_constants_;
        UploadBuffer object_constants_;

		unsigned int num_material_cbs_;
        unsigned int num_object_cbs_;

		BufferManager buffer_manager_;

        Shader* shader_vs_;
        Shader* shader_ps_;
	};
}