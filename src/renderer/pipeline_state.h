#pragma once

#include "util/unordered_map.h"
#include "util/string.h"

#include "renderer/root_signature.h"

namespace blowbox
{
	class Shader;

    /**
    * You actually never directly use this object, it is the base of the GraphicsPSO and the ComputePSO.
    * The PSO only provides functionality that is shared between the other PSO types, such as root signatures
    * and destruction. You are supposed to set all data on the pipeline state, and then calling PSO::Finalize()
    * on it to actually create the PSO.
    *
    * @brief The base PipelineStateObject.
    */
	class PSO
	{
	public:
        /** @brief Constructs the PSO */
		PSO() : root_signature_(nullptr), pso_(nullptr) {}

        /**
        * @brief Sets the root signature in the PSO.
        * @param[in] root_signature The root signature this PSO should use.
        */
		void SetRootSignature(RootSignature& root_signature) { root_signature_ = &root_signature; }

        /** @returns The root signature that was set on this PSO. */
		RootSignature& GetRootSignature() { return *root_signature_; }

        /** @brief Pointer operator overload which returns the underlying PSO. */
		ID3D12PipelineState* operator->() { return pso_; }

        /** @brief Finalizes the PSO - it actually makes it ready for use. */
		virtual void Finalize() = 0;

        /** @brief Destroys the PSO. */
		void Destroy();

        /** @returns The underlying PSO. */
		ID3D12PipelineState* GetPSO() const { return pso_; }

	protected:
		ID3D12PipelineState* pso_;      //!< The underlying PSO object.
		RootSignature* root_signature_; //!< The root signature that is bound to this PSO.
	};

    /**
    * This class inherits from the base PSO and is supposed to help make it simple to
    * create PSO's for graphics pipelines. From the moment of initialisation by the
    * constructor, you should call all the different Setters on this class to configure
    * the PSO properly. Once you have done that, call GraphicsPSO::Finalize() and you
    * are ready to use this PSO in your code.
    *
    * @brief Wraps the PipelineStateObject for graphics pipelines.
    */
	class GraphicsPSO : public PSO
	{
	public:
        /** @brief Constructs the GraphicsPSO. */
		GraphicsPSO();

        /** @brief Destructs the GraphicsPSO. */
        ~GraphicsPSO();

	public:
        /** 
        * @brief Sets the blend state for this PSO.
        * @param[in] blend_desc The blend state description.
        */
		void SetBlendState(const D3D12_BLEND_DESC& blend_desc);

        /**
        * @brief Sets the rasterizer state for this PSO.
        * @param[in] rasterizer_desc The rasterizer state description.
        */
		void SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizer_desc);

        /**
        * @brief Sets the depth stencil state for this PSO.
        * @param[in] depth_stencil_desc The depth stencil state description.
        */
		void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depth_stencil_desc);

        /**
        * @brief Sets the sample mask for this PSO.
        * @param[in] sample_mask The sample mask that should be set.
        */
		void SetSampleMask(UINT sample_mask);

        /**
        * @brief Sets the primitive topology type for this PSO.
        * @param[in] topology_type The topology type that should be set.
        */
		void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topology_type);

        /**
        * @brief Sets the render target in this PSO. This will make it so that this PSO only uses ONE render target.
        * @param[in] rtv_format The rtv format.
        * @param[in] dsv_format The dsv format. Use DXGI_FORMAT_UNKNOWN to unbind.
        * @param[in] msaa_count The MSAA count on the rtv.
        * @param[in] msaa_quality The quality of the MSAA applied.
        */
		void SetRenderTargetFormat(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format, UINT msaa_count = 1, UINT msaa_quality = 0);

        /**
        * @brief Sets the render targets in this PSO. Every format represents one render target.
        * @param[in] num_rtvs The number of RTVs.
        * @param[in] rtv_formats An array of RTV formats.
        * @param[in] dsv_format The DSV format to use. Use DXGI_FORMAT_UNKNOWN to unbind.
        * @param[in] msaa_count The MSAA count on the rtv.
        * @param[in] msaa_quality The quality of the MSAA applied.
        */
		void SetRenderTargetFormats(UINT num_rtvs, const DXGI_FORMAT* rtv_formats, DXGI_FORMAT dsv_format, UINT msaa_count = 1, UINT msaa_quality = 0);

        /**
        * @brief Sets the input layout on this PSO.
        * @param[in] num_elements The number of input elements.
        * @param[in] input_element_descs An array of input element descriptions.
        */
		void SetInputLayout(UINT num_elements, const D3D12_INPUT_ELEMENT_DESC* input_element_descs);

        /**
        * @brief Sets the primitive restart (whatever that means)
        * @param[in] ib_props Not a clue what this means, figure it out yourself.
        */
		void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE ib_props);

        /**
        * @brief Sets this PSO's vertex shader.
        * @param[in] binary The vertex shader binary.
        */
		void SetVertexShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.VS = binary; }

        /**
        * @brief Sets this PSO's pixel shader.
        * @param[in] binary The pixel shader binary.
        */
		void SetPixelShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.PS = binary; }

        /**
        * @brief Sets this PSO's geometry shader.
        * @param[in] binary The geometry shader binary.
        */
		void SetGeometryShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.GS = binary; }

        /**
        * @brief Sets this PSO's hull shader.
        * @param[in] binary The hull shader binary.
        */
		void SetHullShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.HS = binary; }

        /**
        * @brief Sets this PSO's domain shader.
        * @param[in] binary The domain shader binary.
        */
		void SetDomainShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.DS = binary; }

        /** @brief Finalizes the PSO by actually creating it. */
		void Finalize() override;

        /** @returns The PSO description for this PSO. */
        const D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetDesc();
	protected:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc_;       //!< The PSO description.
		D3D12_INPUT_ELEMENT_DESC input_element_descs_[16];  //!< The input element descriptions.
	};

    /**
    * This class inherits from the base PSO and is supposed to help make it simple to
    * create PSO's for compute pipelines. From the moment of initialisation by the
    * constructor, you should call all the different Setters on this class to configure
    * the PSO properly. Once you have done that, call ComputePSO::Finalize() and you
    * are ready to use this PSO in your code.
    *
    * @brief Manages the PipelineStateObject for compute pipelines.
    */
	class ComputePSO : public PSO
	{
	public:
        /** @brief Constructs the ComputePSO. */
		ComputePSO();
        /** @brief Destructs the ComputePSO. */
        ~ComputePSO();

	public:
        /**
        * @brief Sets the compute shader on this ComputePSO.
        * @param[in] binary The compiled shader data.
        * @param[in] size The size of the shader binary.
        */
		void SetComputeShader(const void* binary, size_t size) { pso_desc_.CS = CD3DX12_SHADER_BYTECODE(binary, size); }

        /**
        * @brief Sets the compute shader on this ComputePSO.
        * @param[in] binary The compute shader binary.
        */
		void SetComputeShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.CS = binary; }

        /** @brief Finalizes the PSO by actually creating it. */
		void Finalize() override;

        /** @returns The PSO description for this PSO. */
        const D3D12_COMPUTE_PIPELINE_STATE_DESC& GetDesc();
	protected:
		D3D12_COMPUTE_PIPELINE_STATE_DESC pso_desc_; //!< The PSO description.
	};
}