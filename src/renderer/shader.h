#pragma once

#include "content/text_file.h"
#include "renderer/d3d12_includes.h"

namespace blowbox
{
    /**
    * This enumeration is necessary for the Shader to understand what it
    * is trying to compile. It enumerates all different shader types.
    *
    * @brief Enumeration for all different shader types
    */
    enum ShaderType
    {
        ShaderType_VERTEX,
        ShaderType_PIXEL,
        ShaderType_GEOMETRY,
        ShaderType_COMPUTE,
        ShaderType_HULL,
        ShaderType_DOMAIN,
        ShaderType_UNKNOWN
    };

    /**
    * Provided a TextFile, this class can compile a shader for
    * you and make it usable throughout the rest of the engine.
    * Call Shader::Create() and it will automatically be available
    * for use to you.
    *
    * @brief Wraps Shader objects.
    */
	class Shader
	{
	public:
        /** @brief Constructs a Shader. */
		Shader();

        /** @brief Destructs a Shader. */
		~Shader();

        /** 
        * @brief Creates the actual Shader.
        * @param[in] shader_file The file in which the plain-text shader code is stored.
        * @param[in] shader_type The type of shader that should be created out of the shader_file.
        */
		void Create(TextFile* shader_file, const ShaderType& shader_type);
		
        /** @returns The type of this Shader. */
        const ShaderType& GetShaderType() const;

        /** @returns The bytecode for this Shader.*/
        const D3D12_SHADER_BYTECODE& GetShaderByteCode() const;

        /** @returns The raw binary for this Shader. */
        const void* GetShaderBinary() const;

        /** @returns The length of the raw binary for this Shader. */
        UINT GetShaderBinaryLength() const;

        /** @returns The underlying ID3DBlob in which this Shader is stored. */
        const ID3DBlob* GetShaderBlob() const;

	private:
		TextFile* shader_file_;                     //!< The TextFile that was used to compile this Shader.
		ShaderType shader_type_;                    //!< The type of Shader this is.
        ID3DBlob* shader_blob_;                     //!< The underlying ID3DBlob where the Shader binary is stored in.
		D3D12_SHADER_BYTECODE shader_byte_code_;    //!< A D3D12 compatible shader byte code object for this Shader.
	};
}