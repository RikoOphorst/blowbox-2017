#pragma once

#include "core/debug/debug_window.h"
#include "util/string.h"
#include "util/vector.h"
#include "util/map.h"
#include "renderer/imgui/imgui.h"
#include "renderer/d3d12_includes.h"

#define BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT 2000
#define BLOWBOX_PROFILER_HISTORY_MIN_SAMPLE_COUNT 2

namespace blowbox
{
    class GpuResource;

    class MemoryProfiler : public DebugWindow
    {
        friend class GpuResource;
    public:
        MemoryProfiler();
        ~MemoryProfiler();

        /** @brief Renders the menu for the Profiler. */
        void RenderMenu() override;

        /** @brief Renders the actual window for the Profiler. */
        void RenderWindow() override;

        /** @brief Starts a new profiling frame. */
        void NewFrame();

        /** @brief Shuts down the profiler. */
        void Shutdown();

    protected:
        void AddGpuResource(GpuResource* gpu_resource);
        void RemoveGpuResource(GpuResource* gpu_resource);

        enum ResourceSortType
        {
            ResourceSortType_NAME,
            ResourceSortType_NAME_REVERTED,
            ResourceSortType_SIZE,
            ResourceSortType_SIZE_REVERTED,
            ResourceSortType_DIMENSION,
            ResourceSortType_DIMENSION_REVERTED,
            ResourceSortType_FORMAT,
            ResourceSortType_FORMAT_REVERTED
        };

        struct ResourceData
        {
            GpuResource* resource;
            WString name;
            uint64_t total_size;
            D3D12_RESOURCE_DIMENSION dimension;
            DXGI_FORMAT format;
            uint32_t total_count;
        };

    protected:
        void Condense(bool force = false);
        void Uncondense();

        String ConvertDimensionToString(D3D12_RESOURCE_DIMENSION dimension);
        String ConvertFormatToString(DXGI_FORMAT format);

    private:
        bool has_shutdown_;
        bool show_window_;                                                                  //!< Whether the memory usage window should be shown.
        bool condense_;
        bool is_condensed_;

        Vector<GpuResource*> unsorted_gpu_resources_;
        Map<WString, ResourceData> gpu_resources_map_;
        Vector<ResourceData> sorted_gpu_resources_;

        ImGuiTextFilter filter_;
        ResourceSortType sort_type_;

    protected:
        struct CompareGpuResourceByName
        {
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.name < b.name);
            }
        };

        struct CompareGpuResourceByNameReverted
        {
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.name > b.name);
            }
        };

        struct CompareGpuResourceBySize
        {
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.total_size < b.total_size);
            }
        };

        struct CompareGpuResourceBySizeReverted
        {
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.total_size > b.total_size);
            }
        };

        struct CompareGpuResourceByDimension
        {
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.dimension < b.dimension);
            }
        };

        struct CompareGpuResourceByDimensionReverted
        {
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.dimension > b.dimension);
            }
        };

        struct CompareGpuResourceByFormat
        {
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.format < b.format);
            }
        };

        struct CompareGpuResourceByFormatReverted
        {
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.format > b.format);
            }
        };
    };
}