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

    /**
    * The MemoryProfiler is a debug functionality that allows you to gain insights
    * into how VRAM is being utilized by the application.
    *
    * @brief Provides memory profiling statistics.
    */
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
        /** @brief Adds a GpuResource to the memory profiler. */
        void AddGpuResource(GpuResource* gpu_resource);

        /** @brief Removes a GpuResource from the memory profiler. */
        void RemoveGpuResource(GpuResource* gpu_resource);

        /**
        * @brief The different sorting types in which resources can be sorted in the MemoryProfiler.
        */
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

        /**
        * When there is more than one instance of a GpuResource with a name,
        * this will help condensing all of the GpuResource data into a single
        * struct.
        *
        * @brief Used for condensing data into a single struct inside of the MemoryProfiler.
        */
        struct ResourceData
        {
            GpuResource* resource;              //!< The resource this ResourceData describes.
            WString name;                       //!< The name of the resource being described.
            uint64_t total_size;                //!< The total (condensed) size of the different ResourceData.
            D3D12_RESOURCE_DIMENSION dimension; //!< Dimension of the struct.
            DXGI_FORMAT format;                 //!< The format of the resource being described.
            uint32_t total_count;               //!< The number of GpuResources that are condensed in this ResourceData.
        };

    protected:
        /**
        * @brief Condenses all GpuResources in the MemoryProfiler into single entries.
        * @param[in] force Whether the condense operation should be forced. By default, it is off - because it only needs to condense if anything changed.
        */
        void Condense(bool force = false);

        /**
        * @brief Uncondenses all the GpuResources in the MemoryProfiler.
        */
        void Uncondense();

        /**
        * @brief Converts a D3D12_RESOURCE_DIMENSION to a readable string.
        * @param[in] dimension The dimension to be converted.
        */
        String ConvertDimensionToString(D3D12_RESOURCE_DIMENSION dimension);

        /**
        * @brief Converts a DXGI_FORMAT to a readable string.
        * @param[in] format The format to be converted.
        */
        String ConvertFormatToString(DXGI_FORMAT format);

    private:
        bool has_shutdown_;                             //!< Whether the memory profiler has shutdown.
        bool show_window_;                              //!< Whether the memory profiler window should be shown.
        bool condense_;                                 //!< Whether the different GpuResources in the MemoryProfiler should be condensed or not.
        bool is_condensed_;                             //!< Whether the different GpuResources in the MemoryProfiler are condensed.

        Vector<GpuResource*> unsorted_gpu_resources_;   //!< An unsorted array of GpuResources. New entries are added to this array.
        Map<WString, ResourceData> gpu_resources_map_;  //!< A map of resource names to ResourceData objects. This map is used to store both the condensed and uncondensed resource data.
        Vector<ResourceData> sorted_gpu_resources_;     //!< A sorted array of ResourceDatas, based on the gpu_resources_map_. Used by the ImGui MemoryProfiler window to display the ResourceData in the correct order.

        ImGuiTextFilter filter_;                        //!< A text filter that is used to filter out ResourceDatas by name from the sorted_gpu_resources_ array.
        ResourceSortType sort_type_;                    //!< The sorting type that is used to sort the gpu resources in the sorted_gpu_resources_ array.

    protected:
        /** @brief Helper struct to compare ResourceData by its name. */
        struct CompareGpuResourceByName
        {
            /** 
            * @brief Compares two resource datas.
            * @param[in] a The first ResourceData.
            * @param[in] b The second ResourceData.
            */
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.name < b.name);
            }
        };

        /** @brief Helper struct to compare ResourceData by its name, in the opposite order. */
        struct CompareGpuResourceByNameReverted
        {
            /**
            * @brief Compares two resource datas.
            * @param[in] a The first ResourceData.
            * @param[in] b The second ResourceData.
            */
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.name > b.name);
            }
        };

        /** @brief Helper struct to compare ResourceData by its total size. */
        struct CompareGpuResourceBySize
        {
            /**
            * @brief Compares two resource datas.
            * @param[in] a The first ResourceData.
            * @param[in] b The second ResourceData.
            */
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.total_size < b.total_size);
            }
        };

        /** @brief Helper struct to compare ResourceData by its total size, in the opposite order. */
        struct CompareGpuResourceBySizeReverted
        {
            /**
            * @brief Compares two resource datas.
            * @param[in] a The first ResourceData.
            * @param[in] b The second ResourceData.
            */
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.total_size > b.total_size);
            }
        };

        /** @brief Helper struct to compare ResourceData by its dimension. */
        struct CompareGpuResourceByDimension
        {
            /**
            * @brief Compares two resource datas.
            * @param[in] a The first ResourceData.
            * @param[in] b The second ResourceData.
            */
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.dimension < b.dimension);
            }
        };

        /** @brief Helper struct to compare ResourceData by its dimension, in the opposite order. */
        struct CompareGpuResourceByDimensionReverted
        {
            /**
            * @brief Compares two resource datas.
            * @param[in] a The first ResourceData.
            * @param[in] b The second ResourceData.
            */
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.dimension > b.dimension);
            }
        };

        /** @brief Helper struct to compare ResourceData by its format. */
        struct CompareGpuResourceByFormat
        {
            /**
            * @brief Compares two resource datas.
            * @param[in] a The first ResourceData.
            * @param[in] b The second ResourceData.
            */
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.format < b.format);
            }
        };

        /** @brief Helper struct to compare ResourceData by its format, in the opposite order. */
        struct CompareGpuResourceByFormatReverted
        {
            /**
            * @brief Compares two resource datas.
            * @param[in] a The first ResourceData.
            * @param[in] b The second ResourceData.
            */
            inline bool operator() (ResourceData& a, ResourceData& b)
            {
                return (a.format > b.format);
            }
        };
    };
}