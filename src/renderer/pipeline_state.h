#pragma once

#include "core/eastl.h"
#include <EASTL/unordered_map.h>
#include <EASTL/string.h>

#include "renderer/root_signature.h"

namespace blowbox
{
	class Shader;

	class PSO
	{
	public:
		PSO() : root_signature_(nullptr), pso_(nullptr) {}

		void SetRootSignature(RootSignature& root_signature) { root_signature_ = &root_signature; }
		RootSignature& GetRootSignature() { return *root_signature_; }

		ID3D12PipelineState* operator->() { return pso_; }

		virtual void Finalize() = 0;

		void Destroy();

		ID3D12PipelineState* GetPSO() const { return pso_; }

	protected:
		ID3D12PipelineState* pso_;
		RootSignature* root_signature_;
	};

	class GraphicsPSO : public PSO
	{
	public:
		GraphicsPSO();
        ~GraphicsPSO();

	public:
		static GraphicsPSO& Get(const eastl::string& name);

		void SetBlendState(const D3D12_BLEND_DESC& blend_desc);
		void SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizer_desc);
		void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depth_stencil_desc);
		void SetSampleMask(UINT sample_mask);
		void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topology_type);
		void SetRenderTargetFormat(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format, UINT msaa_count = 1, UINT msaa_quality = 0);
		void SetRenderTargetFormats(UINT num_rtvs, const DXGI_FORMAT* rtv_formats, DXGI_FORMAT dsv_format, UINT msaa_count = 1, UINT msaa_quality = 0);
		void SetInputLayout(UINT num_elements, const D3D12_INPUT_ELEMENT_DESC* input_element_descs);
		void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE ib_props);

		void SetVertexShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.VS = binary; }
		void SetPixelShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.PS = binary; }
		void SetGeometryShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.GS = binary; }
		void SetHullShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.HS = binary; }
		void SetDomainShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.DS = binary; }

		void Finalize() override;
	protected:
		static eastl::unordered_map<eastl::string, GraphicsPSO> psos_;
		
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc_;
		D3D12_INPUT_ELEMENT_DESC input_element_descs_[16];
	};

	class ComputePSO : public PSO
	{
	public:
		ComputePSO();
        ~ComputePSO();

	public:
		static ComputePSO& Get(const eastl::string& name);

		void SetComputeShader(const void* binary, size_t size) { pso_desc_.CS = CD3DX12_SHADER_BYTECODE(binary, size); }
		void SetComputeShader(const D3D12_SHADER_BYTECODE& binary) { pso_desc_.CS = binary; }

		void Finalize() override;
	protected:
		static eastl::unordered_map<eastl::string, ComputePSO> psos_;

		D3D12_COMPUTE_PIPELINE_STATE_DESC pso_desc_;
	};
}