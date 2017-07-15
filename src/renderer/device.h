#pragma once

#include "adapter.h"
#include "renderer/d3d12_includes.h"
#include "util/vector.h"

namespace blowbox
{
    /**
    * This object manages the ID3D12Device object. It automatically creates and deletes it.
    *
    * @brief Wraps the ID3D12Device.
    */
	class Device
	{
	public:
        /** @brief Constructs a Device. */
		Device();

        /** @brief Destructs a Device. */
		~Device();

        /**
        * @brief Creates the actual Device.
        * @remarks Because no Adapter has to be provided for this function, the Device will use the default Adapter.
        */
		void Create();

        /**
        * @brief Creates the actual Device.
        * @param[in] adapter The Adapter the Device should be created with.
        */
		void Create(const Adapter& adapter);

        /** @returns The underlying Adapter. */
        const Adapter& GetAdapter() const;

        /** @returns The underlying ID3D12Device. */
        ID3D12Device* Get() const;

	private:
		Adapter adapter_;       //!< The underlying Adapter that was used to construct the Device.
		ID3D12Device* device_;  //!< The underlying ID3D12Device.
	};
}