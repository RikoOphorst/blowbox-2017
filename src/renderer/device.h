#pragma once

#include "adapter.h"
#include "renderer/d3d12_includes.h"
#include "core/eastl_required.h"
#include <EASTL/vector.h>

namespace blowbox
{
    enum FindAdapterFlag
    {
        FindAdapterFlag_NO_SOFTWARE = 0x1,
        FindAdapterFlag_NO_INTEL = 0x2
    };

	class Device
	{
	public:
		Device();
		~Device();

		void Create();
		void Create(const Adapter& adapter);

        const Adapter& GetAdapter() const;
        ID3D12Device* Get() const;

        static eastl::vector<Adapter> FindAvailableAdapters(FindAdapterFlag find_adapter_flags);

	private:
		Adapter adapter_;
		ID3D12Device* device_;
	};
}