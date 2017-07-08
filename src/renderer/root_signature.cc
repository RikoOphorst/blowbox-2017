#include "root_signature.h"

#include "core/get.h"
#include "renderer.h"
#include "device.h"
#include "util/release.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	RootSignature::RootSignature() :
		num_initialized_root_parameters_(0),
		num_initialized_static_samplers_(0),
		num_root_parameters_(0),
		num_static_samplers_(0),
		root_signature_(nullptr),
		finalized_(false)
	{
		for (int i = 0; i < ROOT_SIGNATURE_MAX_PARAMETERS; i++)
		{
			root_parameters_[i] = RootParameter(this);
		}

		for (int i = 0; i < ROOT_SIGNATURE_MAX_STATIC_SAMPLERS; i++)
		{
			static_samplers_[i] = {};
		}
	}

	//------------------------------------------------------------------------------------------------------
	RootSignature::~RootSignature()
	{
		BLOWBOX_RELEASE(root_signature_);
	}

	//------------------------------------------------------------------------------------------------------
	void RootSignature::Create(UINT num_root_parameters, UINT num_static_samplers)
	{
		assert(num_root_parameters <= ROOT_SIGNATURE_MAX_PARAMETERS);
		assert(num_static_samplers <= ROOT_SIGNATURE_MAX_STATIC_SAMPLERS);

		num_initialized_root_parameters_ = 0;
		num_initialized_static_samplers_ = 0;
		num_root_parameters_ = num_root_parameters;
		num_static_samplers_ = num_static_samplers;
		root_signature_ = nullptr;
		finalized_ = false;

		for (int i = 0; i < ROOT_SIGNATURE_MAX_PARAMETERS; i++)
		{
			root_parameters_[i].Initialize(this);
		}

		for (int i = 0; i < ROOT_SIGNATURE_MAX_STATIC_SAMPLERS; i++)
		{
			static_samplers_[i] = {};
		}
	}

	//------------------------------------------------------------------------------------------------------
	void RootSignature::Finalize(const eastl::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS flags)
	{
		assert(!finalized_); // signatures shouldn't be refinalized, this will break PSOs

		D3D12_ROOT_SIGNATURE_DESC root_desc;
		root_desc.Flags = flags;
		root_desc.NumParameters = num_root_parameters_;
		root_desc.NumStaticSamplers = num_static_samplers_;

		D3D12_ROOT_PARAMETER root_params[ROOT_SIGNATURE_MAX_PARAMETERS];
		for (UINT i = 0; i < num_root_parameters_; i++)
		{
			root_params[i] = root_parameters_[i];
		}

		root_desc.pParameters = root_params;
		root_desc.pStaticSamplers = static_samplers_;

		ID3DBlob* output_blob;
		ID3DBlob* error_blob;

		if (D3D12SerializeRootSignature(&root_desc, D3D_ROOT_SIGNATURE_VERSION_1, &output_blob, &error_blob) != S_OK)
		{
			//DLOG(static_cast<char*>(error_blob->GetBufferPointer()));
			DebugBreak();
		}

		//BLOWBOX_ASSERT_HR(Get::Device().Get()->CreateRootSignature(0, output_blob->GetBufferPointer(), output_blob->GetBufferSize(), IID_PPV_ARGS(&root_signature_)));
		BLOWBOX_ASSERT_HR(Get::Renderer()->GetDevice().Get()->CreateRootSignature(0, output_blob->GetBufferPointer(), output_blob->GetBufferSize(), IID_PPV_ARGS(&root_signature_)));

		root_signature_->SetName(name.c_str());
		finalized_ = true;
	}

	//------------------------------------------------------------------------------------------------------
	RootParameter& RootSignature::operator[](UINT i)
	{
		assert(i < num_root_parameters_); // index cannot exceed number of root params
		return root_parameters_[i];
	}

	//------------------------------------------------------------------------------------------------------
	const RootParameter& RootSignature::operator[](UINT i) const
	{
		assert(i < num_root_parameters_); // index cannot exceed number of root params
		return root_parameters_[i];
	}

	//------------------------------------------------------------------------------------------------------
	void RootSignature::InitStaticSampler(const D3D12_STATIC_SAMPLER_DESC& static_sampler_desc, D3D12_SHADER_VISIBILITY visibility)
	{
		assert(num_initialized_static_samplers_ < num_static_samplers_);
		static_samplers_[num_initialized_static_samplers_++] = static_sampler_desc;
	}

	//------------------------------------------------------------------------------------------------------
	void RootSignature::InitStaticSampler(UINT shader_register, const D3D12_SAMPLER_DESC& sampler_desc, D3D12_SHADER_VISIBILITY visibility)
	{
		assert(num_initialized_static_samplers_ < num_static_samplers_);
		D3D12_STATIC_SAMPLER_DESC& static_sampler_desc = static_samplers_[num_initialized_static_samplers_++];

		static_sampler_desc.Filter = sampler_desc.Filter;
		static_sampler_desc.AddressU = sampler_desc.AddressU;
		static_sampler_desc.AddressV = sampler_desc.AddressV;
		static_sampler_desc.AddressW = sampler_desc.AddressW;
		static_sampler_desc.MipLODBias = sampler_desc.MipLODBias;
		static_sampler_desc.MaxAnisotropy = sampler_desc.MaxAnisotropy;
		static_sampler_desc.ComparisonFunc = sampler_desc.ComparisonFunc;
		static_sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		static_sampler_desc.MinLOD = sampler_desc.MinLOD;
		static_sampler_desc.MaxLOD = sampler_desc.MaxLOD;
		static_sampler_desc.ShaderRegister = shader_register;
		static_sampler_desc.RegisterSpace = 0;
		static_sampler_desc.ShaderVisibility = visibility;

		if (static_sampler_desc.AddressU == D3D12_TEXTURE_ADDRESS_MODE_BORDER ||
			static_sampler_desc.AddressV == D3D12_TEXTURE_ADDRESS_MODE_BORDER ||
			static_sampler_desc.AddressW == D3D12_TEXTURE_ADDRESS_MODE_BORDER)
		{
			if (sampler_desc.BorderColor[0] == 0.0f &&
				sampler_desc.BorderColor[1] == 0.0f &&
				sampler_desc.BorderColor[2] == 0.0f &&
				sampler_desc.BorderColor[3] == 0.0f ||
				// Opaque Black
				sampler_desc.BorderColor[0] == 0.0f &&
				sampler_desc.BorderColor[1] == 0.0f &&
				sampler_desc.BorderColor[2] == 0.0f &&
				sampler_desc.BorderColor[3] == 1.0f ||
				// Opaque White
				sampler_desc.BorderColor[0] == 1.0f &&
				sampler_desc.BorderColor[1] == 1.0f &&
				sampler_desc.BorderColor[2] == 1.0f &&
				sampler_desc.BorderColor[3] == 1.0f
				)
			{
				//DLOG("Sampler border color does not match static sampler limitations, defaulting to fallback values..");
			}

			if (sampler_desc.BorderColor[3] == 1.0f)
			{
				if (sampler_desc.BorderColor[0] == 1.0f)
				{
					static_sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
				}
				else
				{
					static_sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
				}
			}
			else
			{
				static_sampler_desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	void RootSignature::ConfirmInitializedParameter()
	{
		num_initialized_root_parameters_++;
	}
	
	//------------------------------------------------------------------------------------------------------
	RootParameter::RootParameter()
	{
	}

	//------------------------------------------------------------------------------------------------------
	RootParameter::RootParameter(RootSignature* owning_signature) :
		owning_signature_(owning_signature)
	{

	}

	//------------------------------------------------------------------------------------------------------
	RootParameter::~RootParameter()
	{
		Clear();
	}
	
	//------------------------------------------------------------------------------------------------------
	void RootParameter::Initialize(RootSignature* owning_signature)
	{
		Clear();
		owning_signature_ = owning_signature;
	}

	//------------------------------------------------------------------------------------------------------
	void RootParameter::Clear()
	{
		if (ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
		{
			delete[] DescriptorTable.pDescriptorRanges;
		}

		ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	//------------------------------------------------------------------------------------------------------
	void RootParameter::InitAsConstants(UINT shader_register, UINT num_dwords, D3D12_SHADER_VISIBILITY visibility)
	{
		ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		ShaderVisibility = visibility;
		Constants.Num32BitValues = num_dwords;
		Constants.ShaderRegister = shader_register;
		Constants.RegisterSpace = 0;
		owning_signature_->ConfirmInitializedParameter();
	}

	//------------------------------------------------------------------------------------------------------
	void RootParameter::InitAsConstantBuffer(UINT shader_register, D3D12_SHADER_VISIBILITY visibility)
	{
		ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		ShaderVisibility = visibility;
		Descriptor.ShaderRegister = shader_register;
		Descriptor.RegisterSpace = 0;
		owning_signature_->ConfirmInitializedParameter();
	}

	//------------------------------------------------------------------------------------------------------
	void RootParameter::InitAsBufferSRV(UINT shader_register, D3D12_SHADER_VISIBILITY visibility)
	{
		ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		ShaderVisibility = visibility;
		Descriptor.ShaderRegister = shader_register;
		Descriptor.RegisterSpace = 0;
		owning_signature_->ConfirmInitializedParameter();
	}

	//------------------------------------------------------------------------------------------------------
	void RootParameter::InitAsBufferUAV(UINT shader_register, D3D12_SHADER_VISIBILITY visibility)
	{
		ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		ShaderVisibility = visibility;
		Descriptor.ShaderRegister = shader_register;
		Descriptor.RegisterSpace = 0;
		owning_signature_->ConfirmInitializedParameter();
	}

	//------------------------------------------------------------------------------------------------------
	void RootParameter::InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT count, UINT shader_register, D3D12_SHADER_VISIBILITY visibility)
	{
		InitAsDescriptorTable(1, visibility);
		SetTableRange(0, type, shader_register, count);
		owning_signature_->ConfirmInitializedParameter();
	}

	//------------------------------------------------------------------------------------------------------
	void RootParameter::InitAsDescriptorTable(UINT range_count, D3D12_SHADER_VISIBILITY visibility)
	{
		ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		ShaderVisibility = visibility;
		DescriptorTable.NumDescriptorRanges = range_count;
		DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[range_count];
	}

	//------------------------------------------------------------------------------------------------------
	void RootParameter::SetTableRange(UINT range_index, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shader_register, UINT count, UINT space)
	{
		D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(DescriptorTable.pDescriptorRanges + range_index);
		range->RangeType = type;
		range->NumDescriptors = count;
		range->BaseShaderRegister = shader_register;
		range->RegisterSpace = space;
		range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		owning_signature_->ConfirmInitializedParameter();
	}
}