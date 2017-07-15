#pragma once

#include "renderer/d3d12_includes.h"
#include "util/string.h"
#include "util/vector.h"

namespace blowbox
{
    /**
    * These flags are used by the Adapter::FindAvailableAdapters(). They help
    * in specifying what adapter should be used.
    *
    * @brief Flags that are provided to Adapter::FindAvailableAdapters()
    */
    enum FindAdapterFlag
    {
        FindAdapterFlag_NO_SOFTWARE = 0x1,
        FindAdapterFlag_NO_INTEL = 0x2
    };

    /**
    * This struct wraps all data for Adapters and makes it easily
    * available to the user to read.
    *
    * @brief Wraps an IDXGIAdapter.
    */
    struct Adapter
    {
        /** @brief Constructs an empty Adapter. */
        Adapter();

        /**
        * @brief Constructs an Adapter.
        * @param[in] dxgi_adapter The actual adapter this Adapter wraps.
        */
        Adapter(IDXGIAdapter1* dxgi_adapter);

        IDXGIAdapter4* dxgi_adapter;                //!< The underlying IDXGIAdapter.
        String name;                                //!< The name of this Adapter.
        DXGI_ADAPTER_FLAG3 flags;                   //!< Any flags that are set on the Adapter
        unsigned long long video_memory;            //!< How much video memory is available on this Adapter
        unsigned long long system_memory;           //!< How much system memory is available on this Adapter
        unsigned long long shared_system_memory;    //!< How much shared system memory is available on this Adapter
        LUID adapter_luid;                          //!< The LUID of this Adapter.

        /**
        * @brief Find a list of available adapters.
        * @param[in] find_adapter_flags Any find adapter flags you might want to pass in
        * @returns A list of adapters that can be used to create the Device with.
        */
        static Vector<Adapter> FindAvailableAdapters(FindAdapterFlag find_adapter_flags);
    };
}