#pragma once

#include "d3d12_includes.h"

#include "util/string.h"
#include "util/weak_ptr.h"

#define BLOWBOX_DESCRIPTOR_ID_UNKNOWN ~(0u)

namespace blowbox
{
    typedef unsigned int DescriptorId;

	class Device;

	class DescriptorHeap
	{
	public:
		DescriptorHeap();
		~DescriptorHeap();

		void Create(const WString& name, D3D12_DESCRIPTOR_HEAP_TYPE heap_type, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, UINT descriptor_count = 4096U);

		/**
		* @brief Creates a DepthStencilView (descriptor)
		* @param[in] device The device used to create the descriptor & heaps with
		* @param[in] dsv_buffer The buffer this descriptor should describe
		* @param[in] desc The description of this descriptor used to describe the resource with
		*/
		UINT CreateDepthStencilView(ID3D12Resource* dsv_buffer, D3D12_DEPTH_STENCIL_VIEW_DESC* desc);

		/**
		* @brief Creates a RenderTargetView (descriptor)
		* @param[in] device The device used to create the descriptor & heaps with
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

		ID3D12DescriptorHeap* Get() { return heap_; }
		const D3D12_DESCRIPTOR_HEAP_DESC& GetDesc() { return heap_desc_; }

		void Clear();
	private:
		ID3D12DescriptorHeap* heap_;
		D3D12_DESCRIPTOR_HEAP_DESC heap_desc_;
		WeakPtr<Device> device_;

		// @todo Make the descriptor heap use a pool strategy instead of linear strategy
		UINT current_allocations_; 
		UINT descriptor_size_;
	};
}