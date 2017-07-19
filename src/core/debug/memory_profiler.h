#pragma once

#include "core/debug/debug_window.h"
#include "util/string.h"
#include "util/map.h"
#include "util/ring_buffer.h"
#include "util/utility.h"
#include "util/vector.h"
#include "renderer/imgui/imgui.h"

#define BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT 2000
#define BLOWBOX_PROFILER_HISTORY_MIN_SAMPLE_COUNT 2

namespace blowbox
{
    class GpuResource;

    class MemoryProfiler : public DebugWindow
    {
    public:
        MemoryProfiler();
        ~MemoryProfiler();

        /** @brief Renders the menu for the Profiler. */
        void RenderMenu() override;

        /** @brief Renders the actual window for the Profiler. */
        void RenderWindow() override;

        /** @brief Starts a new profiling frame. */
        void NewFrame();

    private:
        float last_bounds_reset_time_;                                                      //!< The last time that bounds got reset in the profiler.
        float bounds_reset_interval_;                                                       //!< The interval at which bounds should be reset.
        int history_sample_count_;                                                          //!< The number of samples that should be kept as history for all history buffers.

    private:
        bool show_window_;                                                                  //!< Whether the memory usage window should be shown.

        RingBuffer<uint64_t> ram_usage_history_;                                            //!< A history of how much RAM was in use.
        float ram_usage_history_contiguous_[BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT];     //!< An array used to contiguously store the RAM usage history.
        uint64_t ram_usage_history_upper_bound_;                                            //!< The upper bound of the RAM usage (i.e. the most RAM that has been in use).
        uint64_t ram_usage_history_lower_bound_;                                            //!< The lower bound of the RAM usage (i.e. the fewest RAM that has been in use).
        uint64_t ram_operating_system_budget_;                                              //!< Stores how much RAM is still available for allocation on this machine.
        uint64_t ram_average_usage_;                                                        //!< Stores the average amount of RAM usage based on the current RAM usage history.

        RingBuffer<uint64_t> vram_usage_history_;                                           //!< A history of how much VRAM was in use.
        float vram_usage_history_contiguous_[BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT];    //!< An array used to contiguously store the VRAM usage history.
        uint64_t vram_usage_history_upper_bound_;                                           //!< The upper bound of the VRAM usage (i.e. the most VRAM that has been in use).
        uint64_t vram_usage_history_lower_bound_;                                           //!< The lower bound of the VRAM usage (i.e. the fewest VRAM that has been in use).
        uint64_t vram_operating_system_budget_;                                             //!< Stores how much VRAM is still available for allocation on this machine before the application starts throttling.
        uint64_t vram_average_usage_;                                                       //!< Stores the average amount of VRAM usage based on the current VRAM usage history.
    };
}