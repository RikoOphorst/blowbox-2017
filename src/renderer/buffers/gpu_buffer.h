#pragma once

#include "util/string.h"

#include "gpu_resource.h"

namespace blowbox
{
    /**
    * Any type of buffer resource in Blowbox is based on the GpuBuffer.
    * The GpuBuffer automatically creates the necessary resource descriptors
    * for you when you call GpuBuffer::Create(). You can retrieve those by
    * calling GpuBuffer::GetUAV() or GpuBuffer::GetSRV().
    *
    * @brief Wraps buffer-type GpuResource's.
    */
	class GpuBuffer : public GpuResource
	{
	public:
        /** @brief Destructs the GpuBuffer. */
		virtual ~GpuBuffer() { Destroy(); }

        /** @brief Destroys the GpuBuffer. */
		virtual void Destroy();

        /**
        * @brief Creates actual buffer resource.
        * @param[in] name The name of the buffer resource.
        * @param[in] num_elements The number of elements that exist in the buffer.
        * @param[in] element_size The size of each element in the buffer.
        * @param[in] initial_data If the buffer should be initialized immediately with data, enter a pointer to it here. Pointer should be of size num_elements * element_size.
        * @param[in] create_views Whether the resource descriptors (views) should automatically be created.
        */
		virtual void Create(const WString& name, UINT num_elements, UINT element_size, void* initial_data = nullptr, bool create_views = true);

        /** @returns The UAV for this resource. */
		const UINT& GetUAV() const { return uav_id_; }

        /** @returns The SRV for this resource. */
		const UINT& GetSRV() const { return srv_id_; }

        /** @returns The GPU VRAM address of this resource, which can directly be used as a root CBV via GraphicsContext::SetConstantBuffer(). */
		const D3D12_GPU_VIRTUAL_ADDRESS& GetRootCBV() const { return gpu_virtual_address_; }

        /** @returns The GPU VRAM address of this resource, which can directly be used as a root CBV via GraphicsContext::SetConstantBuffer(). */
		D3D12_GPU_VIRTUAL_ADDRESS GetRootCBV() { return gpu_virtual_address_; }

        /** 
        * @returns A VertexBufferView for this buffer. 
        * @param[in] offset An offset into the buffer from which the VBV should start.
        * @param[in] size The size of the VBV - i.e. how many bytes the VBV should cover. (most likely equal to num_elements * element_size)
        * @param[in] stride The stride size of each vertex in the buffer (most likely equal to element_size)
        */
		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(UINT offset, UINT size, UINT stride) const;

        /** 
        * @returns A VertexBufferView for this buffer, created automatically based on which element should be first.
        * @param[in] base_vertex_index The index of the first vertex that the VBV should base itself around.
        */
		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(UINT base_vertex_index = 0) const;

        /**
        * @returns An IndexBufferView for this buffer.
        * @param[in] offset An offset into the buffer from which the IBV should start.
        * @param[in] size The size of the VBV - i.e. how many bytes the IBV should cover. (most likely equal to num_elements * element_size)
        * @param[in] is_32_bit Whether every index in the buffer is 32 bit or 16 bit. 16 bit usually suffices.
        */
		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(UINT offset, UINT size, bool is_32_bit = false) const;

        /**
        * @returns An IndexBufferView for this buffer, created automatically based on which index should be first.
        * @param[in] start_index The index of the first index that the IBV should base itself around.
        */
		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(UINT start_index = 0) const;

	protected:
        /** @brief Constructs a GpuBuffer. */
		GpuBuffer();

        /** 
        * @brief Generates a D3D12_RESOURCE_DESC based on the buffer size and resource flags.
        * @returns A D3D12_RESOURCE_DESC for buffer-type resources based on this GpuBuffer's settings.
        */
		D3D12_RESOURCE_DESC DescribeBuffer();

        /** @brief Creates the descriptors (views) for this GpuBuffer. */
		virtual void CreateDerivedViews() = 0;

		UINT uav_id_;                           //!< The UAV descriptor for this GpuBuffer.
		UINT srv_id_;                           //!< The SRV descriptor for this GpuBuffer.
		UINT element_count_;                    //!< The number of elements in this GpuBuffer.
		UINT element_size_;                     //!< The size of each element in this GpuBuffer.
		UINT buffer_size_;                      //!< The total buffer size of this GpuBuffer.
		D3D12_RESOURCE_FLAGS resource_flags_;   //!< The resource flags set on this GpuBuffer.
	};
}