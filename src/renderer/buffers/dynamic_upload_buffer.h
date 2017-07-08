#pragma once

namespace blowbox
{
	template<typename T>
	class DynamicUploadBuffer
	{
	public:
		struct BufferData
		{
			ID3D12Resource* buffer;
			BYTE* mapped_data;
		};

		DynamicUploadBuffer(ID3D12Device* device, UINT element_count, bool is_constant_buffer = false)
		{
			element_count_per_buffer_ = element_count;

			element_byte_size_ = sizeof(T);
			if (is_constant_buffer)
			{
				// constant buffers need to have byte size of >=256
				element_byte_size_ = (sizeof(T) + 255) & ~255;
			}

			CreateNewBuffer(device);
		}

		DynamicUploadBuffer(const DynamicUploadBuffer& rhs) = delete;
		DynamicUploadBuffer& operator=(const DynamicUploadBuffer& rhs) = delete;

		~DynamicUploadBuffer()
		{
			for (int i = 0; i < upload_buffers_.size(); i++)
			{
				upload_buffers_[i].buffer->Unmap(0, nullptr);
				upload_buffers_[i].buffer->Release();
				upload_buffers_[i].buffer = nullptr;
				upload_buffers_[i].mapped_data = nullptr;
			}

			upload_buffers_.clear();
		}

		const std::vector<BufferData>& GetBuffers() const { return upload_buffers_; }
		UINT GetElementByteSize() const { return element_byte_size_; }

		void CopyData(ID3D12Device* device, int element_index, const T& data)
		{
			int buffer_index = 0;
			int elem_index = 0;
			FindBufferIndexAndElementIndex(element_index, &buffer_index, &elem_index);

			for (int i = upload_buffers_.size(); i <= buffer_index; i++)
			{
				CreateNewBuffer(device);
			}

			BufferData buf_data = upload_buffers_[buffer_index];

			memcpy(&(buf_data.mapped_data[elem_index * element_byte_size_]), &data, sizeof(T));
		}

		void FindBufferIndexAndElementIndex(int idx, int* buffer_index, int* elem_index)
		{
			*buffer_index = (RoundUpToMultiple(idx + 1, static_cast<int>(element_count_per_buffer_)) - static_cast<int>(element_count_per_buffer_)) / static_cast<int>(element_count_per_buffer_);
			*elem_index = idx % element_count_per_buffer_;
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetAddressOfElement(int idx)
		{
			int buf_idx = 0;
			int elem_idx = 0;
			FindBufferIndexAndElementIndex(idx, &buf_idx, &elem_idx);

			for (int i = upload_buffers_.size(); i <= buf_idx; i++)
			{
				CreateNewBuffer(Renderer::Instance()->GetDevice());
			}

			auto address = upload_buffers_[buf_idx].buffer->GetGPUVirtualAddress();
			address += element_byte_size_ * elem_idx;

			return address;
		}

	private:
		void CreateNewBuffer(ID3D12Device* device)
		{
			BufferData data;

			data.buffer = nullptr;
			data.mapped_data = nullptr;

			BLOWBOX_ASSERT_HR(
				device->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(element_byte_size_ * element_count_per_buffer_),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&(data.buffer))
				)
			);

			BLOWBOX_ASSERT_HR(data.buffer->Map(0, nullptr, reinterpret_cast<void**>(&data.mapped_data)));

			upload_buffers_.push_back(data);
		}

	private:
		std::vector<BufferData> upload_buffers_;

		UINT element_count_;
		UINT element_count_per_buffer_;
		UINT element_byte_size_;
		bool is_constant_buffer_;
	};
}