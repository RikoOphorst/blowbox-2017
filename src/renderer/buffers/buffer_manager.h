#pragma once

#include <unordered_map>

#include "core/eastl_required.h"
#include <EASTL/unordered_map.h>

#include "renderer/buffers/color_buffer.h"
#include "renderer/buffers/depth_buffer.h"
#include "renderer/buffers/byte_address_buffer.h"
#include "renderer/buffers/typed_buffer.h"
#include "renderer/buffers/structured_buffer.h"

namespace blowbox
{
	enum ColorBufferType
	{
		ColorBufferUnknown,
		ColorBufferScene
	};

	enum DepthBufferType
	{
		DepthBufferUnknown,
		DepthBufferScene
	};

	enum ByteBufferType
	{
		ByteBufferUnknown
	};

	enum TypedBufferType
	{
		TypedBufferUnknown
	};

	enum StructuredBufferType
	{
		StructuredBufferUnknown
	};

	class BufferManager
	{
	public:
		BufferManager();
		~BufferManager();

		void Create(UINT width, UINT height);
		void Destroy();

		static ColorBuffer& GetBuffer(const ColorBufferType& type);
		static DepthBuffer& GetBuffer(const DepthBufferType& type);
		static ByteAddressBuffer& GetBuffer(const ByteBufferType& type);
		static TypedBuffer& GetBuffer(const TypedBufferType& type);
		static StructuredBuffer& GetBuffer(const StructuredBufferType& type);
	private:
		std::unordered_map<ColorBufferType, ColorBuffer> buffers_color_;
        std::unordered_map<DepthBufferType, DepthBuffer> buffers_depth_;
        std::unordered_map<ByteBufferType, ByteAddressBuffer> buffers_byte_address_;
        std::unordered_map<TypedBufferType, TypedBuffer> buffers_typed_;
        std::unordered_map<StructuredBufferType, StructuredBuffer> buffers_structured_;
	};
}