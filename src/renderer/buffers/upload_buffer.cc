#include "upload_buffer.h"

#include "core/get.h"
#include "renderer/descriptor_heap.h"
#include "renderer/commands/command_context.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::Create(const eastl::wstring& name, UINT num_elements, UINT element_size, void* initial_data)
	{
		element_count_ = num_elements;
		element_size_ = element_size;
		buffer_size_ = num_elements * element_size;
		usage_state_ = D3D12_RESOURCE_STATE_COMMON;
		
		BLOWBOX_ASSERT_HR(
			Get::Device()->Get()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(element_size_ * element_count_),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&resource_)
			)
		);

		gpu_virtual_address_ = resource_->GetGPUVirtualAddress();

		resource_->SetName(name.c_str());

		is_mapped_ = false;
		Map();

		if (initial_data != nullptr)
		{
			memcpy(mapped_data_, initial_data, buffer_size_);
		}

		CreateDerivedViews();
	}
	
	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::CreateDerivedViews()
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srv_desc.Format = DXGI_FORMAT_UNKNOWN;
		srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv_desc.Buffer.NumElements = element_count_;
		srv_desc.Buffer.StructureByteStride = element_size_;
		srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		srv_id_ = Get::CbvSrvUavHeap()->CreateShaderResourceView(resource_, &srv_desc);

		// resources in an upload heap cannot have a UAV
		uav_id_ = BLOWBOX_DESCRIPTOR_ID_UNKNOWN;
	}

	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::Destroy()
	{
		Unmap();
		GpuResource::Destroy();
	}

	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::Map()
	{
		if (!is_mapped_)
		{
			BLOWBOX_ASSERT_HR(resource_->Map(0, nullptr, reinterpret_cast<void**>(&mapped_data_)));
			is_mapped_ = true;
		}
	}

	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::Unmap()
	{
		if (is_mapped_)
		{
			resource_->Unmap(0, nullptr);
			is_mapped_ = false;
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::InsertDataByElement(UINT element_id, void* data)
	{
		memcpy(&mapped_data_[element_id * element_size_], data, element_size_);
	}

	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::ClearDataByElement(UINT element_id)
	{
		memset(&mapped_data_[element_id * element_size_], 0, element_size_);
	}

	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::InsertDataByBuffer(void* data, UINT size_of_data_in_bytes)
	{
		memcpy(mapped_data_, data, static_cast<size_t>(size_of_data_in_bytes));
	}

	//------------------------------------------------------------------------------------------------------
	void UploadBuffer::ClearDataByBuffer()
	{
		memset(mapped_data_, 0, element_count_ * element_size_);
	}
	
	//------------------------------------------------------------------------------------------------------
	D3D12_GPU_VIRTUAL_ADDRESS UploadBuffer::GetAddressByElement(UINT element_id)
	{
		auto address = resource_->GetGPUVirtualAddress();
		address += element_size_ * element_id;

		return address;
	}
}