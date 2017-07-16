#include "shader.h"

#include "util/assert.h"
#include "util/release.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	Shader::Shader() :
        shader_file_(nullptr),
		shader_blob_(nullptr),
        shader_type_(ShaderType_UNKNOWN)
	{
		ZeroMemory(&shader_byte_code_, sizeof(D3D12_SHADER_BYTECODE));
	}
	
	//------------------------------------------------------------------------------------------------------
	Shader::~Shader()
	{
		BLOWBOX_RELEASE(shader_blob_);
	}
	
	//------------------------------------------------------------------------------------------------------
	void Shader::Create(TextFile* shader_file, const ShaderType& shader_type)
	{
        BLOWBOX_ASSERT(shader_file != nullptr);
        BLOWBOX_ASSERT(shader_type != ShaderType_UNKNOWN);

        shader_file_ = shader_file;
        shader_type_ = shader_type;

		eastl::string entry_point;
        eastl::string shader_model;
		switch (shader_type_)
		{
		case ShaderType_VERTEX:
			entry_point = "main";
			shader_model = "vs_5_0";
			break;
		case ShaderType_GEOMETRY:
			entry_point = "main";
			shader_model = "gs_5_0";
			break;
        case ShaderType_PIXEL:
            entry_point = "main";
            shader_model = "ps_5_0";
            break;
        case ShaderType_COMPUTE:
            entry_point = "main";
            shader_model = "cs_5_0";
            break;
		}

		UINT flags = NULL;
#if defined(_DEBUG) || defined(DEBUG)
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

		ID3DBlob* shader_blob_intermediate = nullptr;
		ID3DBlob* error_blob_intermediate = nullptr;
		HRESULT hr = D3DCompile(
            shader_file_->GetFileContent().c_str(), 
            shader_file_->GetFileContent().size(),
            NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, 
            entry_point.c_str(), 
            shader_model.c_str(), 
            flags, NULL,
            &shader_blob_intermediate, 
            &error_blob_intermediate
        );

		if (hr != S_OK || error_blob_intermediate != nullptr)
		{
			//DLOG("Shader compile failed:");
			//DLOG(static_cast<const char*>(error_blob_->GetBufferPointer()));

            OutputDebugStringA(static_cast<char*>(error_blob_intermediate->GetBufferPointer()));

            BLOWBOX_ASSERT(false);
            BLOWBOX_RELEASE(error_blob_intermediate);
			
            return;
		}
		else
		{
			shader_blob_ = shader_blob_intermediate;
			shader_byte_code_.BytecodeLength = shader_blob_->GetBufferSize();
			shader_byte_code_.pShaderBytecode = shader_blob_->GetBufferPointer();
		}
	}

    //------------------------------------------------------------------------------------------------------
    const ShaderType& Shader::GetShaderType() const
    {
        return shader_type_;
    }

    //------------------------------------------------------------------------------------------------------
    const D3D12_SHADER_BYTECODE& Shader::GetShaderByteCode() const
    {
        return shader_byte_code_;
    }

    //------------------------------------------------------------------------------------------------------
    const void* Shader::GetShaderBinary() const
    {
        return shader_byte_code_.pShaderBytecode;
    }

    //------------------------------------------------------------------------------------------------------
    UINT Shader::GetShaderBinaryLength() const
    {
        return static_cast<UINT>(shader_byte_code_.BytecodeLength);
    }

    //------------------------------------------------------------------------------------------------------
    const ID3DBlob* Shader::GetShaderBlob() const
    {
        return shader_blob_;
    }
}