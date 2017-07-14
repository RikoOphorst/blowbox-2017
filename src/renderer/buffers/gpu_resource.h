#pragma once

#include "renderer/d3d12_includes.h"

#define D3D12_RESOURCE_STATE_INVALID ((D3D12_RESOURCE_STATES)-1)

namespace blowbox
{
    /**
    * This GpuResource class wraps the entire ID3D12Resource object. It is
    * useful because it keeps track of the current resource state it currently
    * is in and allows for efficient state switching by CommandContexts.
    * Every resource in Blowbox should be wrapped by this class. If you want
    * access to the underlying ID3D12Resource, you can use GpuResource::Get().
    *
    * @brief Wraps ID3D12Resource objects.
    */
	class GpuResource
	{
		friend class CommandContext;
	public:
        /** @brief Constructs an empty GpuResource. */
		GpuResource();
        
        /**
        * @brief Constructs a GpuResource that wraps a certain ID3D12Resource object.
        * @param[in] resource The resource that needs to be wrapped.
        * @param[in] current_state The state of the resource that needs to be wrapped.
        */
		GpuResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES current_state);

        /** @brief Destructs a GpuResource. */
		virtual ~GpuResource();

        /**
        * @brief Associate this GpuResource with another ID3D12Resource.
        * @param[in] resource The resource that this GpuResource should associate itself with.
        * @param[in] current_state The state of the resource that this GpuResource should associate itself with.
        */
		void Associate(ID3D12Resource* resource, D3D12_RESOURCE_STATES current_state);

        /** @brief Destroys the underlying GpuResource. */
		void Destroy();

        /** @brief Pointer operator that returns the underlying resource. */
		ID3D12Resource* operator->() { return resource_; }

        /** @brief Pointer operator that returns the underlying resource. */
		const ID3D12Resource* operator->() const { return resource_; }

        /** @brief Implicitly convert this GpuResource to a ID3D12Resource. */
		operator ID3D12Resource*() { return resource_; }

        /** @brief Returns the underlying ID3D12Resource. */
		ID3D12Resource* Get() { return resource_; }

        /**
        * @brief Returns the current resource state of the resource.
        * @returns The current resource state of the resource.
        */
		const D3D12_RESOURCE_STATES& GetState() const { return usage_state_; }

        /**
        * @brief Returns the current transition state of the resource.
        * @returns The current transition state of the resource.
        * @remarks The transition state is used by the CommandContext in order to queue up resource transitions. 
        *          A resource transition can be triggered on a GpuResource, but it doesn't need to be immediately
        *          transitioned to the new resource state. While it is in that limbo-state, this is will return 
        *          the state that is going to transition towards.
        */
		const D3D12_RESOURCE_STATES& GetTransitionState() const { return transition_state_; }

	protected:
		ID3D12Resource* resource_;                      //!< The ID3D12Resource that is being wrapped by the GpuResource.
		D3D12_RESOURCE_STATES usage_state_;             //!< The current resource state of the ID3D12Resource.
		D3D12_RESOURCE_STATES transition_state_;        //!< The current transition state of the GpuResource.
		D3D12_GPU_VIRTUAL_ADDRESS gpu_virtual_address_; //!< The virtual address of the ID3D12Resource in VRAM.
	};
}