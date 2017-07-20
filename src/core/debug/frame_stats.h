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

    /**
    * Provides a simple debug window with frame statistics in the main debug menu.
    *
    * @brief Provides an overview of frame statistics.
    */
    class FrameStats : public DebugWindow
    {
    public:
        FrameStats();
        ~FrameStats();

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
        bool show_window_;                                                                  //!< Whether the frame stats window should be shown.
        RingBuffer<float> delta_time_history_;                                              //!< A history buffer for all delta times.
        float delta_time_history_contiguous_[BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT];    //!< Stores the delta times from the delta_time_history_ contiguously in memory.
        float delta_times_upper_bound_;                                                     //!< The upper bound of the delta times (i.e. the highest delta time) that exist in the current delta_time_history_.
        float delta_times_lower_bound_;                                                     //!< The lower bound of the delta times (i.e. the lowest delta time) that exist in the current delta_time_history_.
        float average_delta_time_;                                                          //!< The average delta time of the delta times inside of the current delta_time_history_.

        float fps_history_contiguous_[BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT];           //!< Stores FPS values contiguously in memory.

        int view_frame_stats_as_fps_;                                                       //!< Whether stats in the frame stats window should be shown as delta time or as FPS.
    };
}