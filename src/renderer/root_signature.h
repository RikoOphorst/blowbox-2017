#pragma once

#include "core/eastl_required.h"
#include <EASTL/string.h>
#include "d3d12_includes.h"

#define ROOT_SIGNATURE_MAX_PARAMETERS 64
#define ROOT_SIGNATURE_MAX_STATIC_SAMPLERS 64

namespace blowbox
{
	class RootSignature;

	class RootParameter : public D3D12_ROOT_PARAMETER
	{
		friend class RootSignature;
	protected:
		RootParameter();
		RootParameter(RootSignature* owning_signature);
		~RootParameter();

		void Initialize(RootSignature* owning_signature);
		void Clear();

	public:		
		void InitAsConstants(UINT shader_register, UINT num_dwords, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void InitAsConstantBuffer(UINT shader_register, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void InitAsBufferSRV(UINT shader_register, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void InitAsBufferUAV(UINT shader_register, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT count, UINT shader_register, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void InitAsDescriptorTable(UINT range_count, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void SetTableRange(UINT range_index, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shader_register, UINT count, UINT space = 0);

	private:
		RootSignature* owning_signature_;
	};

	class RootSignature
	{
		friend class RootParameter;
	public:
		RootSignature();
		~RootSignature();

		void Create(UINT num_root_parameters, UINT num_static_samplers);
		void Finalize(const eastl::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

		RootParameter& operator[](UINT i);
		const RootParameter& operator[](UINT i) const;

		ID3D12RootSignature* Get() const { return root_signature_; }
		operator ID3D12RootSignature*() const { return root_signature_; }
		ID3D12RootSignature* operator->() const { return root_signature_; }

		void InitStaticSampler(const D3D12_STATIC_SAMPLER_DESC& static_sampler_desc, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
		void InitStaticSampler(UINT shader_register, const D3D12_SAMPLER_DESC& sampler_desc, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

	protected:
		void ConfirmInitializedParameter();

	private:
		bool finalized_;
		UINT num_root_parameters_;
		UINT num_initialized_root_parameters_;
		UINT num_static_samplers_;
		UINT num_initialized_static_samplers_;
		ID3D12RootSignature* root_signature_;
		RootParameter root_parameters_[ROOT_SIGNATURE_MAX_PARAMETERS];
		D3D12_STATIC_SAMPLER_DESC static_samplers_[ROOT_SIGNATURE_MAX_STATIC_SAMPLERS];
	};
}