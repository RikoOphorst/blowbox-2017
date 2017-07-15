#pragma once

#include "util/string.h"
#include "d3d12_includes.h"

#define ROOT_SIGNATURE_MAX_PARAMETERS 64
#define ROOT_SIGNATURE_MAX_STATIC_SAMPLERS 64

namespace blowbox
{
	class RootSignature;

    /**
    * This class wraps the functionality of a D3D12_ROOT_PARAMETER. It's goal
    * is to make it simple and straightforward to configure RootParameter in
    * an elegant way. Call one of the InitAs* functions on this to initialize
    * the RootParameter as something.
    *
    * @brief Makes it easy to configure a D3D12_ROOT_PARAMETER.
    */
	class RootParameter : public D3D12_ROOT_PARAMETER
	{
		friend class RootSignature;
	protected:
        /** @brief Constructs a RootParameter. */
		RootParameter();
        
        /** 
        * @brief Constructs a RootParameter.
        * @param[in] owning_signature The root signature that owns this RootParameter.
        */
		RootParameter(RootSignature* owning_signature);
		
        /** @brief Destructs this RootParameter. */
        ~RootParameter();

        /**
        * @brief Initializes the root parameter.
        * @param[in] owning_signature The signature that owns this parameter.
        */
		void Initialize(RootSignature* owning_signature);

        /** @brief Clears the root parameter. */
		void Clear();

	public:
        /**
        * @brief Initializes this RootParameter as root constants.
        * @param[in] shader_register The register at which these constants should appear in the shaders.
        * @param[in] num_dwords The number of 32 bit values that are in this root parameter.
        * @param[in] visibility The shader stages in which this root parameter is visible.
        */
		void InitAsConstants(UINT shader_register, UINT num_dwords, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

        /**
        * @brief Initializes this RootParameter as a root constant buffer.
        * @param[in] shader_register The register at which the root constant buffer should appear in the shaders.
        * @param[in] visibility The shader stages in which this root parameter is visible.
        */
		void InitAsConstantBuffer(UINT shader_register, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

        /**
        * @brief Initializes this RootParameter as a root SRV buffer.
        * @param[in] shader_register The register at which the root SRV buffer should appear in the shaders.
        * @param[in] visibility The shader stages in which this root parameter is visible.
        */
		void InitAsBufferSRV(UINT shader_register, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

        /**
        * @brief Initializes this RootParameter as a root UAV buffer.
        * @param[in] shader_register The register at which the root UAV buffer should appear in the shaders.
        * @param[in] visibility The shader stages in which this root parameter is visible.
        */
		void InitAsBufferUAV(UINT shader_register, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

        /**
        * @brief Initializes this RootParameter as a descriptor range (actually it gets initialized as a descriptor table with only one range in it).
        * @param[in] type The type of descriptor range this should be (UAV, SRV, CBV, SAMPLER).
        * @param[in] count The size of the descriptor range, should be at least 1.
        * @param[in] shader_register The register at which the descriptor range should appear in the shaders.
        * @param[in] visibility The shader stages in which this root parameter is visible.
        */
		void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT count, UINT shader_register, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

        /**
        * @brief Initializes this RootParameter as a descriptor table.
        * @param[in] range_count The number of descriptor ranges that this descriptor table controls.
        * @param[in] visibility The shader stages in which this root parameter is visible.
        * @remarks This doesn't initializes the different descriptor ranges in this descriptor table. Use RootParameter::SetTableRange() for that.
        */
		void InitAsDescriptorTable(UINT range_count, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

        /**
        * @brief Sets a descriptor range inside of this RootParameter (which should have been initialized as a DescriptorTable before!)
        * @param[in] range_index The index of the descriptor range in the descriptor table that you want to set.
        * @param[in] type The type of descriptor range this should be (UAV, SRV, CBV, SAMPLER).
        * @param[in] shader_register The register at which the descriptor range should appear in the shaders.
        * @param[in] count The size of the descriptor range, should be at least 1.
        * @param[in] space The register space of this descriptor range.
        */
		void SetDescriptorRangeInTable(UINT range_index, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shader_register, UINT count, UINT space = 0);

	private:
		RootSignature* owning_signature_; //!< The RootSignature that owns this RootParameter
	};


    /**
    * This class makes it easy for you to create RootSignatures by cutting out a lot
    * of the fuss from setting up all the different root parameters and also wrapping
    * the logic for finalizing the root signature. When you want to create a RootSignature,
    * start by calling RootSignature::Create() with the number of root parameters and
    * static samplers that you want. Then continue to initialize all the root parameters
    * by using the [] operator to access each RootParameter. After that, continue to 
    * initialize the static samplers that you want to use by calling RootSignature::InitStaticSampler().
    * After all that is set and done, finalize the RootSignature by calling RootSignature::Finalize().
    * Your RootSignature is now ready for use.
    *
    * @brief Wraps the ID3D12RootSignature object.
    */
	class RootSignature
	{
		friend class RootParameter;
	public:
        /** @brief Constructs the RootSignature. */
		RootSignature();

        /** @brief Destructs the RootSignature. */
		~RootSignature();

        /** 
        * @brief Creates all the underlying root parameters and static samplers, so that you can access them with the [] operator.
        * @param[in] num_root_parameters The number of root parameters that should be in this RootSignature.
        * @param[in] num_static_samplers The number of static samplers that should be in this RootSignature.
        */
		void Create(UINT num_root_parameters, UINT num_static_samplers);

        /**
        * @brief Finalizes the RootSignature. It will validate all RootParameters and construct the final ID3D12RootSignature.
        * @param[in] name The name of this RootSignature.
        * @param[in] flags Any flags that this RootSignature should be created with.
        */
		void Finalize(const WString& name, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

        /** @returns The ith RootParameter. */
		RootParameter& operator[](UINT i);

        /** @returns The ith RootParameter. */
		const RootParameter& operator[](UINT i) const;

        /** @returns The underlying ID3D12RootSignature. */
		ID3D12RootSignature* Get() const { return root_signature_; }

        /** @brief Implicit casting to ID3D12RootSignature. */
		operator ID3D12RootSignature*() const { return root_signature_; }

        /** @brief Overload the pointer operator to access the underlying ID3D12RootSignature. */
		ID3D12RootSignature* operator->() const { return root_signature_; }

        /** 
        * @brief Init a static sampler with a given static sampler desc. 
        * @param[in] static_sampler_desc The static sampler desc
        * @param[in] visibility The shader stages in which this sampler is visible.
        * @remarks It is preferred to use the other overload of this function to make it simpler for you to initialize static samplers.
        */
		void InitStaticSampler(const D3D12_STATIC_SAMPLER_DESC& static_sampler_desc, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

        /**
        * @brief Init a static sampler with a given NON-static sampler desc.
        * @param[in] shader_register The shader register where this sampler should appear in the shaders.
        * @param[in] sampler_desc The NON-static sampler desc
        * @param[in] visibility The shader stages in which this sampler is visible.
        */
		void InitStaticSampler(UINT shader_register, const D3D12_SAMPLER_DESC& sampler_desc, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

	protected:
        /**
        * @brief Confirms to the RootSignature that a RootParameter has been successfully initialized.
        * @remarks This function is only called from the RootParameter itself after it was successfully initialized.
        */
		void ConfirmInitializedParameter();

	private:
		bool finalized_;                                                                //!< Whether the RootSignature has been finalized.
		UINT num_root_parameters_;                                                      //!< The number of RootParameters in this RootSignature.
		UINT num_initialized_root_parameters_;                                          //!< The number of currently initialized RootParameters.
		UINT num_static_samplers_;                                                      //!< The number of static samplers in this RootSignature.
		UINT num_initialized_static_samplers_;                                          //!< The number of currently initialized static samplers.
		ID3D12RootSignature* root_signature_;                                           //!< The underlying ID3D12RootSignature.
		RootParameter root_parameters_[ROOT_SIGNATURE_MAX_PARAMETERS];                  //!< All the RootParameters in this RootSignature.
		D3D12_STATIC_SAMPLER_DESC static_samplers_[ROOT_SIGNATURE_MAX_STATIC_SAMPLERS]; //!< All the static sampllers in this RootSignature.
	};
}