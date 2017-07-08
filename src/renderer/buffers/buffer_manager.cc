#include "buffer_manager.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	BufferManager::BufferManager()
	{

	}

	//------------------------------------------------------------------------------------------------------
	BufferManager::~BufferManager()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void BufferManager::Create(UINT width, UINT height)
	{
		buffers_color_[ColorBufferScene].Create(L"BufferSceneColor", width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
		buffers_depth_[DepthBufferScene].Create(L"BufferSceneDepth", width, height, DXGI_FORMAT_D32_FLOAT);
	}

	//------------------------------------------------------------------------------------------------------
	void BufferManager::Destroy()
	{
		buffers_color_.clear();
		buffers_depth_.clear();
		buffers_byte_address_.clear();
		buffers_typed_.clear();
		buffers_structured_.clear();
	}
	
	//------------------------------------------------------------------------------------------------------
	ColorBuffer& BufferManager::GetBuffer(const ColorBufferType& type)
	{
		return Get::BufferManager().buffers_color_[type];
	}

	//------------------------------------------------------------------------------------------------------
	DepthBuffer& BufferManager::GetBuffer(const DepthBufferType& type)
	{
		return Get::BufferManager().buffers_depth_[type];
	}

	//------------------------------------------------------------------------------------------------------
	ByteAddressBuffer& BufferManager::GetBuffer(const ByteBufferType& type)
	{
		return Get::BufferManager().buffers_byte_address_[type];
	}

	//------------------------------------------------------------------------------------------------------
	TypedBuffer& BufferManager::GetBuffer(const TypedBufferType& type)
	{
		return Get::BufferManager().buffers_typed_[type];
	}

	//------------------------------------------------------------------------------------------------------
	StructuredBuffer& BufferManager::GetBuffer(const StructuredBufferType& type)
	{
		return Get::BufferManager().buffers_structured_[type];
	}
}