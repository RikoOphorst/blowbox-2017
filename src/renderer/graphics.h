#pragma once

#include "root_signature.h"

namespace blowbox
{
	namespace Graphics
	{
		D3D12_STATIC_SAMPLER_DESC sampler_static_point_wrap;
		D3D12_STATIC_SAMPLER_DESC sampler_static_point_clamp;
		D3D12_STATIC_SAMPLER_DESC sampler_static_linear_wrap;
		D3D12_STATIC_SAMPLER_DESC sampler_static_linear_clamp;
		D3D12_STATIC_SAMPLER_DESC sampler_static_anisotropic_wrap;
		D3D12_STATIC_SAMPLER_DESC sampler_static_anisotropic_clamp;

		D3D12_SAMPLER_DESC sampler_point_wrap;
		D3D12_SAMPLER_DESC sampler_point_clamp;
		D3D12_SAMPLER_DESC sampler_linear_wrap;
		D3D12_SAMPLER_DESC sampler_linear_clamp;
		D3D12_SAMPLER_DESC sampler_anisotropic_wrap;
		D3D12_SAMPLER_DESC sampler_anisotropic_clamp;

		D3D12_RASTERIZER_DESC rasterizer_default;
		D3D12_RASTERIZER_DESC rasterizer_default_cw;
		D3D12_RASTERIZER_DESC rasterizer_no_culling;
		D3D12_RASTERIZER_DESC rasterizer_wireframe;
		D3D12_RASTERIZER_DESC rasterizer_no_culling_wireframe;

		D3D12_DEPTH_STENCIL_DESC depth_state_default;
		D3D12_DEPTH_STENCIL_DESC depth_state_disabled;
		D3D12_DEPTH_STENCIL_DESC depth_state_read_write;
		D3D12_DEPTH_STENCIL_DESC depth_state_read_only;
		D3D12_DEPTH_STENCIL_DESC depth_state_read_reversed;
		D3D12_DEPTH_STENCIL_DESC depth_state_read_test_equal;

		D3D12_BLEND_DESC blend_state_disabled;
		D3D12_BLEND_DESC blend_state_no_color_write;
		D3D12_BLEND_DESC blend_state_pre_multiplied;
		D3D12_BLEND_DESC blend_state_traditional;
		D3D12_BLEND_DESC blend_state_additive;
		D3D12_BLEND_DESC blend_state_traditional_additive;

		RootSignature root_signature_default;

		void Initialize()
		{
			sampler_static_point_wrap = CD3DX12_STATIC_SAMPLER_DESC(
				0, 
				D3D12_FILTER_MIN_MAG_MIP_POINT, 
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, 
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, 
				D3D12_TEXTURE_ADDRESS_MODE_WRAP
			);

			sampler_static_point_clamp = CD3DX12_STATIC_SAMPLER_DESC(
				1, 
				D3D12_FILTER_MIN_MAG_MIP_POINT, 
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP
			);

			sampler_static_linear_wrap = CD3DX12_STATIC_SAMPLER_DESC(
				2, 
				D3D12_FILTER_MIN_MAG_MIP_LINEAR,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, 
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, 
				D3D12_TEXTURE_ADDRESS_MODE_WRAP
			);

			sampler_static_linear_clamp = CD3DX12_STATIC_SAMPLER_DESC(
				3, 
				D3D12_FILTER_MIN_MAG_MIP_LINEAR, 
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP
			);

			sampler_static_anisotropic_wrap = CD3DX12_STATIC_SAMPLER_DESC(
				4, 
				D3D12_FILTER_ANISOTROPIC, 
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, 
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, 
				D3D12_TEXTURE_ADDRESS_MODE_WRAP, 
				0.0f, 
				8
			);

			sampler_static_anisotropic_clamp = CD3DX12_STATIC_SAMPLER_DESC(
				5, 
				D3D12_FILTER_ANISOTROPIC, 
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 
				0.0f, 
				8
			);

			sampler_point_wrap.BorderColor[0] = 0.0f;
			sampler_point_wrap.BorderColor[1] = 0.0f;
			sampler_point_wrap.BorderColor[2] = 0.0f;
			sampler_point_wrap.BorderColor[3] = 0.0f;
			sampler_point_wrap.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			sampler_point_wrap.MinLOD = 0.0f;
			sampler_point_wrap.MaxLOD = D3D12_FLOAT32_MAX;
			sampler_point_wrap.MipLODBias = 0.0f;
			sampler_point_wrap.MaxAnisotropy = 8;
			sampler_point_wrap.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			sampler_point_wrap.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler_point_wrap.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler_point_wrap.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

			sampler_point_clamp = sampler_point_wrap;
			sampler_point_clamp.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler_point_clamp.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler_point_clamp.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

			sampler_linear_wrap = sampler_point_wrap;
			sampler_linear_wrap.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

			sampler_linear_clamp = sampler_point_clamp;
			sampler_linear_clamp.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

			sampler_anisotropic_wrap = sampler_point_wrap;
			sampler_anisotropic_wrap.Filter = D3D12_FILTER_ANISOTROPIC;

			sampler_anisotropic_clamp = sampler_point_clamp;
			sampler_anisotropic_clamp.Filter = D3D12_FILTER_ANISOTROPIC;

			rasterizer_default.FillMode = D3D12_FILL_MODE_SOLID;
			rasterizer_default.CullMode = D3D12_CULL_MODE_BACK;
			rasterizer_default.FrontCounterClockwise = TRUE;
			rasterizer_default.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			rasterizer_default.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			rasterizer_default.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			rasterizer_default.DepthClipEnable = TRUE;
			rasterizer_default.MultisampleEnable = FALSE;
			rasterizer_default.AntialiasedLineEnable = FALSE;
			rasterizer_default.ForcedSampleCount = 0;
			rasterizer_default.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

			rasterizer_default_cw = rasterizer_default;
			rasterizer_default_cw.FrontCounterClockwise = FALSE;

			rasterizer_no_culling = rasterizer_default;
			rasterizer_no_culling.CullMode = D3D12_CULL_MODE_NONE;
			rasterizer_no_culling.DepthClipEnable = FALSE;

			rasterizer_wireframe = rasterizer_default;
			rasterizer_wireframe.FillMode = D3D12_FILL_MODE_WIREFRAME;

			rasterizer_no_culling_wireframe = rasterizer_default;
			rasterizer_no_culling_wireframe.CullMode = D3D12_CULL_MODE_NONE;
			rasterizer_no_culling_wireframe.FillMode = D3D12_FILL_MODE_WIREFRAME;

			depth_state_default.DepthEnable = TRUE;
			depth_state_default.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			depth_state_default.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			depth_state_default.StencilEnable = FALSE;
			depth_state_default.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
			depth_state_default.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

			depth_state_disabled.DepthEnable = FALSE;
			depth_state_disabled.DepthEnable = FALSE;
			depth_state_disabled.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			depth_state_disabled.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			depth_state_disabled.StencilEnable = FALSE;
			depth_state_disabled.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
			depth_state_disabled.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
			depth_state_disabled.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			depth_state_disabled.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
			depth_state_disabled.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
			depth_state_disabled.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
			depth_state_disabled.BackFace = depth_state_disabled.FrontFace;

			depth_state_read_write = depth_state_disabled;
			depth_state_read_write.DepthEnable = TRUE;
			depth_state_read_write.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			depth_state_read_write.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

			depth_state_read_only = depth_state_read_write;
			depth_state_read_only.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

			D3D12_DEPTH_STENCIL_DESC prepass_depth_state;
			prepass_depth_state.DepthEnable = TRUE;
			prepass_depth_state.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			prepass_depth_state.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			prepass_depth_state.StencilEnable = FALSE;
			prepass_depth_state.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
			prepass_depth_state.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
			prepass_depth_state.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			prepass_depth_state.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
			prepass_depth_state.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
			prepass_depth_state.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
			prepass_depth_state.BackFace = prepass_depth_state.FrontFace;

			depth_state_read_reversed = depth_state_read_only;
			depth_state_read_reversed.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;

			depth_state_read_test_equal = depth_state_read_only;
			depth_state_read_test_equal.DepthFunc = D3D12_COMPARISON_FUNC_EQUAL;

			depth_state_default = depth_state_read_write;

			D3D12_BLEND_DESC alpha_blend = {};
			alpha_blend.IndependentBlendEnable = FALSE;
			alpha_blend.RenderTarget[0].BlendEnable = FALSE;
			alpha_blend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			alpha_blend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			alpha_blend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			alpha_blend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			alpha_blend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			alpha_blend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			alpha_blend.RenderTarget[0].RenderTargetWriteMask = 0;
			blend_state_no_color_write = alpha_blend;

			alpha_blend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blend_state_disabled = alpha_blend;

			alpha_blend.RenderTarget[0].BlendEnable = TRUE;
			blend_state_traditional = alpha_blend;

			alpha_blend.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			blend_state_pre_multiplied = alpha_blend;

			alpha_blend.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blend_state_additive = alpha_blend;

			alpha_blend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blend_state_traditional_additive = alpha_blend;

			root_signature_default.Create(4, 6);
			root_signature_default[0].InitAsConstantBuffer(0);
			root_signature_default[1].InitAsConstantBuffer(1);
			root_signature_default[2].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); // diffuse
			root_signature_default.InitStaticSampler(0, sampler_anisotropic_wrap);
			root_signature_default.Finalize(L"RootSignatureDefault", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		}
	}
}