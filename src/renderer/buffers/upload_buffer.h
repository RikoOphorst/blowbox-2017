#pragma once

#include "renderer/d3d12_includes.h"
#include "renderer/device.h"
#include "renderer/buffers/gpu_buffer.h"
#include "renderer/buffers/byte_address_buffer.h"

#include "core/eastl_required.h"
#include "EASTL/string.h"

namespace blowbox
{
	/**
	* @class blowbox::UploadBuffer
	* @brief A buffer type which can be used as a way to very easily upload data to the GPU
	* @author Riko Ophorst
	*/
	class UploadBuffer : public GpuBuffer
	{
	public:
		/**
		* @brief Creates the upload buffer
		* @param[in] name The name of the buffer you're creating. This will be used internally to easily identify the buffer
		* @param[in] num_elements The number of elements that should be able to fit in this buffer
		* @param[in] element_size The size of each elements that should be able to fit in this buffer
		* @param[in] initial_data (optional) The data that this buffer should be initialized with
		*/
		void Create(const eastl::wstring& name, UINT num_elements, UINT element_size, void* initial_data = nullptr);

		virtual void CreateDerivedViews() override; //!< Creates any views that this buffer type may have
		virtual void Destroy() override; //!< Destroys this resource and clears it from GPU memory

		void Map(); //!< "Maps" the resource from GPU memory to CPU memory, making it writable & readable (although reading is strongly advised against!)
		void Unmap(); //!< "Unmaps" the resource from CPU memory to GPU memory, making it non-writable & non-readable

		/**
		* @brief Insert data into the buffer by a certain element index. Make sure the resource has been Map()-ed to CPU memory (by default, all UploadBuffers are mapped)
		* @param[in] element_id The index (id) of the element you want to write to
		* @param[in] data The data you want to write into the element
		*/
		void InsertDataByElement(UINT element_id, void* data);

		/**
		* @brief Clear all data that resides in an element and set it to 0x0
		* @param[in] element_id The index (id) of the element you want to clear
		*/
		void ClearDataByElement(UINT element_id);

		/**
		* @brief Set ALL data in the UploadBuffer to what you pass in. Throws an error when (size_of_data_in_bytes != element_size * element_count)
		* @param[in] element_id The index (id) of the element you want to clear
		* @param[in] size_of_data_in_bytes The size of the data you are writing into the buffer
		*/
		void InsertDataByBuffer(void* data, UINT size_of_data_in_bytes);

		/**
		* @brief Clear all data that resides in the entire buffer and set it to 0x0
		*/
		void ClearDataByBuffer();

		/**
		* @brief Query for the GPU memory address of an element in the UploadBuffer
		* @param[in] element_id The index (id) of the element you want the address of
		*/
		D3D12_GPU_VIRTUAL_ADDRESS GetAddressByElement(UINT element_id);
	private:
		bool is_mapped_ = false; //!< Is the buffer currently mapped to CPU memory?
		BYTE* mapped_data_ = nullptr; //!< The data that is mapped to CPU memory - can be freely written to and read from when it is mapped
	};
}