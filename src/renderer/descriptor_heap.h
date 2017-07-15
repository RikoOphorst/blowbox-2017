#pragma once

#include "d3d12_includes.h"

#include "util/string.h"
#include "util/weak_ptr.h"

#define BLOWBOX_DESCRIPTOR_ID_UNKNOWN ~(0u)

namespace blowbox
{
    //typedef unsigned int DescriptorId;

	class Device;

    /**
    * Helps managing ID3D12DescriptorHeaps. It makes it easy to create and access descriptors for resources.
    *
    * @brief Manages a ID3D12DescriptorHeap of a certain type.
    */
	class DescriptorHeap
	{
	public:
        /** @brief Constructs a DescriptorHeap. */
		DescriptorHeap();
        /** @brief Destructs the DescriptorHeap. */
		~DescriptorHeap();

        /**
        * @brief Creates the DescriptorHeap.
        * @param[in] name The name of the DescriptorHeap.
        * @param[in] heap_type The type of DescriptorHeap.
        * @param[in] flags Any flags that should be set on the DescriptorHeap.
        * @param[in] descriptor_count The number of descriptors this DescriptorHeap should be able to hold.
        */
		void Create(const WString& name, D3D12_DESCRIPTOR_HEAP_TYPE heap_type, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, UINT descriptor_count = 4096U);

		/**
		* @brief Creates a DepthStencilView (descriptor)
		* @param[in] dsv_buffer The buffer this descriptor should describe
		* @param[in] desc The description of this descriptor used to describe the resource with
		*/
		UINT CreateDepthStencilView(ID3D12Resource* dsv_buffer, D3D12_DEPTH_STENCIL_VIEW_DESC* desc);

		/**
		* @brief Creates a RenderTargetView (descriptor)
		* @param[in] rtv_buffer The buffer this descriptor should describe
		* @param[in] desc The description of this descriptor used to describe the resource with
		*/
		UINT CreateRenderTargetView(ID3D12Resource* rtv_buffer, D3D12_RENDER_TARGET_VIEW_DESC* desc);

		/**
		* @brief Creates a ConstantBufferView (descriptor)
		* @param[in] desc The description of this descriptor used to describe the resource with
		*/
		UINT CreateConstantBufferView(D3D12_CONSTANT_BUFFER_VIEW_DESC* desc);

		/**
		* @brief Creates a ShaderResourceView (descriptor)
		* @param[in] srv_buffer The buffer the descriptor is describing
		* @param[in] desc The description of this descriptor used to describe the resource with
		*/
		UINT CreateShaderResourceView(ID3D12Resource* srv_buffer, D3D12_SHADER_RESOURCE_VIEW_DESC* desc);

		/**
		* @brief Creates a UnorderedAccessView (descriptor)
		* @param[in] buffer The buffer this descriptor should describe
		* @param[in] counter_buffer The counter buffer this descriptor should describe
		* @param[in] desc The description of this descriptor used to describe the resource with
		*/
		UINT CreateUnorderedAccessView(ID3D12Resource* buffer, ID3D12Resource* counter_buffer, D3D12_UNORDERED_ACCESS_VIEW_DESC* desc);

		/**
		* @brief Creates a sampler state (descriptor)
		* @param[in] desc The description of this descriptor used to describe the resource with
		*/
		UINT CreateSampler(D3D12_SAMPLER_DESC* desc);

		/**
		* @brief Get a cpu handle to a descriptor by its id
		* @param[in] id The id of the desired descriptor
		*/
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorById(UINT id);

		/**
		* @brief Get a gpu handle to a descriptor by its id
		* @param[in] id The id of the desired descriptor
		*/
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorById(UINT id);

        /** @returns The underlying ID3D12DescriptorHeap. */
		ID3D12DescriptorHeap* Get() { return heap_; }

        /** @returns The underlying ID3D12DescriptorHeap. */
		const D3D12_DESCRIPTOR_HEAP_DESC& GetDesc() { return heap_desc_; }

        /** @brief Essentially resets the DescriptorHeap. */
		void Clear();
	private:
		ID3D12DescriptorHeap* heap_;                //!< The underlying descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC heap_desc_;      //!< The description for the descriptor heap.

		// @todo Make the descriptor heap use a pool strategy instead of linear strategy
		UINT current_allocations_;                  //!< The number of descriptors in the descriptor heap in active use.
		UINT descriptor_size_;                      //!< The size of a single descriptor.
	};
}