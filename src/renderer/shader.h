#pragma once

#include "content/text_file.h"
#include "renderer/d3d12_includes.h"

namespace blowbox
{
    enum ShaderType
    {
        ShaderType_VERTEX,
        ShaderType_PIXEL,
        ShaderType_GEOMETRY,
        ShaderType_COMPUTE,
        ShaderType_UNKNOWN
    };

	class Shader
	{
	public:
		Shader();
		~Shader();

		void Create(TextFile* shader_file, const ShaderType& shader_type);
		
        const ShaderType& GetShaderType() const;
        const D3D12_SHADER_BYTECODE& GetShaderByteCode() const;
        const void* GetShaderBinary() const;
        const UINT& GetShaderBinaryLength() const;
        const ID3DBlob* GetShaderBlob() const;

	private:
		TextFile* shader_file_;
		ShaderType shader_type_;
        ID3DBlob* shader_blob_;
		D3D12_SHADER_BYTECODE shader_byte_code_;
	};
}