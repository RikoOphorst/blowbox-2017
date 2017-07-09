#include "pipeline_state.h"

#include "core/get.h"
#include "util/release.h"
#include "renderer/device.h"

namespace blowbox
{
	eastl::unordered_map<eastl::string, GraphicsPSO> GraphicsPSO::psos_ = eastl::unordered_map<eastl::string, GraphicsPSO>();
    eastl::unordered_map<eastl::string, ComputePSO> ComputePSO::psos_ = eastl::unordered_map<eastl::string, ComputePSO>();

	//------------------------------------------------------------------------------------------------------
	void PSO::Destroy()
	{
		BLOWBOX_RELEASE(pso_);
	}
	
	//------------------------------------------------------------------------------------------------------
	GraphicsPSO::GraphicsPSO()
	{
		ZeroMemory(&pso_desc_, sizeof(pso_desc_));
		pso_desc_.NodeMask = 0;
		pso_desc_.SampleMask = 0;
		pso_desc_.SampleDesc.Count = 1;
		pso_desc_.InputLayout.NumElements = 0;
	}

    //------------------------------------------------------------------------------------------------------
    GraphicsPSO::~GraphicsPSO()
    {

    }

	//------------------------------------------------------------------------------------------------------
	GraphicsPSO& GraphicsPSO::Get(const eastl::string& name)
	{
		auto& find = psos_.find(name);

		if (find == psos_.end())
		{
			psos_.emplace(eastl::make_pair(name, GraphicsPSO()));
		}

		return psos_[name];
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetBlendState(const D3D12_BLEND_DESC& blend_desc)
	{
		pso_desc_.BlendState = blend_desc;
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizer_desc)
	{
		pso_desc_.RasterizerState = rasterizer_desc;
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depth_stencil_desc)
	{
		pso_desc_.DepthStencilState = depth_stencil_desc;
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetSampleMask(UINT sample_mask)
	{
		pso_desc_.SampleMask = sample_mask;
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topology_type)
	{
		assert(topology_type != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED); // what are you even trying to do if you want to draw with an undefined topology???
		pso_desc_.PrimitiveTopologyType = topology_type;
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetRenderTargetFormat(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format, UINT msaa_count, UINT msaa_quality)
	{
		SetRenderTargetFormats(1, &rtv_format, dsv_format, msaa_count, msaa_quality);
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetRenderTargetFormats(UINT num_rtvs, const DXGI_FORMAT* rtv_formats, DXGI_FORMAT dsv_format, UINT msaa_count, UINT msaa_quality)
	{
		pso_desc_.NumRenderTargets = num_rtvs;
		memcpy(pso_desc_.RTVFormats, rtv_formats, sizeof(DXGI_FORMAT) * num_rtvs);

		pso_desc_.DSVFormat = dsv_format;
		pso_desc_.SampleDesc.Count = msaa_count;
		pso_desc_.SampleDesc.Quality = msaa_quality;
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetInputLayout(UINT num_elements, const D3D12_INPUT_ELEMENT_DESC* input_element_descs)
	{
		assert(num_elements < 16); // arbitrary limit because of array limits
		pso_desc_.InputLayout.NumElements = num_elements;

		memcpy(input_element_descs_, input_element_descs, sizeof(D3D12_INPUT_ELEMENT_DESC) * num_elements);
		pso_desc_.InputLayout.pInputElementDescs = input_element_descs_;
	}

	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE ib_props)
	{
		pso_desc_.IBStripCutValue = ib_props;
	}
	
	//------------------------------------------------------------------------------------------------------
	void GraphicsPSO::Finalize()
	{
		assert(pso_ == nullptr); // a pso shouldn't be refinalized
		pso_desc_.pRootSignature = root_signature_->Get();
		assert(pso_desc_.pRootSignature != nullptr); // root signature has to be set

		BLOWBOX_ASSERT_HR(Get::Device()->Get()->CreateGraphicsPipelineState(&pso_desc_, IID_PPV_ARGS(&pso_)));
	}
	
	//------------------------------------------------------------------------------------------------------
	ComputePSO::ComputePSO()
	{
		ZeroMemory(&pso_desc_, sizeof(pso_desc_));
		pso_desc_.NodeMask = 1;
	}

	//------------------------------------------------------------------------------------------------------
	ComputePSO& ComputePSO::Get(const eastl::string& name)
	{
		auto& find = psos_.find(name);

		if (find == psos_.end())
		{
			psos_.emplace(eastl::make_pair(name, ComputePSO()));
		}

		return psos_[name];
	}
	
	//------------------------------------------------------------------------------------------------------
	void ComputePSO::Finalize()
	{
		assert(pso_ == nullptr); // a pso shouldn't be refinalized
		pso_desc_.pRootSignature = root_signature_->Get();
		assert(pso_desc_.pRootSignature != nullptr); // root signature has to be set

		Get::Device()->Get()->CreateComputePipelineState(&pso_desc_, IID_PPV_ARGS(&pso_));
	}
}