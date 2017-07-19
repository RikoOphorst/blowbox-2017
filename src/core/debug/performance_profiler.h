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

    /** @brief Identifies a type of profiling block. */
    enum ProfilerBlockType
    {
        ProfilerBlockType_GAME,
        ProfilerBlockType_CORE,
        ProfilerBlockType_RENDERER,
        ProfilerBlockType_CONTENT,
        ProfilerBlockType_MISC,
        ProfilerBlockType_COUNT
    };

    /**
    * This class is here in Blowbox to let you profile your code. You can start
    * profiling a new block of code by calling stack-allocating a PerformanceProfiler::ProfilerBlock.
    * Once the ProfilerBlock has expired, it automatically notifies the main Profiler.
    * See the PerformanceProfiler::ProfilerBlock for a more detailed explanation.
    *
    * @brief The main profiler in Blowbox.
    */
    class PerformanceProfiler : public DebugWindow
    {
    public:
        /**
        * This class is the beating heart of the profiler. It allows you to
        * profile any block of code with literally only one line of code.
        * The ProfilerBlock is all based around RAII and the scope in which
        * it lives. It is automatically initialized when you allocate the
        * memory for it, and it it automatically reported to the Profiler
        * at the end of its lifetime. You only need to pass in a name and a
        * category in which the ProfilerBlock should exist. The Profiler
        * will take care of the rest.
        *
        * @brief Allows you to profile a block of code.
        */
        class ProfilerBlock
        {
            friend class PerformanceProfiler;
            friend class CompareProfilerBlockSingleFrame;
        public:
            ProfilerBlock();

            /**
            * @brief Starts a new ProfilerBlock.
            * @param[in] block_name The name of the block that you're profiling. This name will be used in the Perf Profiler UI to identify this block.
            * @param[in] block_type The type of block that you're profiling. This is optional, but if you'd like to group your ProfilerBlocks in the Perf Profiler UI you can use this.
            */
            ProfilerBlock(const String& block_name, const ProfilerBlockType& block_type = ProfilerBlockType_MISC);

            /** @brief Destructs the ProfilerBlock. It automatically calls ProfilerBlock::Finish() for you. */
            ~ProfilerBlock();

            /** @brief This invalidates the ProfilerBlock. */
            void Restart();

            /** @brief Finishes the ProfilerBlock. This means that the block will expire and report itself to the main Profiler instance. */
            void Finish();
        private:
            String block_name_;             //!< The name of this ProfilerBlock.
            ProfilerBlockType block_type_;  //!< The type of this ProfilerBlock.
            double start_time_;             //!< The time at which this ProfilerBlock spawned.
            double end_time_;               //!< The time at which this ProfilerBlock finished.
            bool finished_;                 //!< Whether the block has been finished already.
        };

        /**
        * This struct describes the data for a type of ProfilerBlock over the
        * lifetime of a frame. If there are multiple ProfilerBlocks with the
        * same name and block type, they will automatically be "grouped" under
        * this struct - it essentially functions as an umbrella for ProfilerBlocks.
        *
        * @brief DO NOT USE! This struct is meant for internal use by the Profiler.
        */
        struct ProfilerBlockFrameData
        {
            double total_time;              //!< The total amount of time that has been spent for this ProfilerBlock in this frame.
            String block_name;              //!< The name of the block.
            ProfilerBlockType block_type;   //!< The type of the block.
        };

        PerformanceProfiler();
        ~PerformanceProfiler();

        /** @brief Renders the menu for the Profiler. */
        void RenderMenu() override;

        /** @brief Renders the actual window for the Profiler. */
        void RenderWindow() override;

        /** @brief Starts a new profiling frame. */
        void NewFrame();

        /** @brief Makes the profiler collect the next frame's data. */
        void CatchNextFrame();

    protected:
        /** @brief This is a lightweight version of the PerformanceProfiler::ProfilerBlock. It only stores the block time. */
        struct ProfilerBlockTime
        {
            double start_time;              //!< The time at which the ProfilerBlock spawned.
            double end_time;                //!< The time at which the ProfilerBlock finished.
        };

        /**
        * @brief Stores expanded data for PerformanceProfiler::ProfilerBlocks, over the lifetime of the entire application.
        */
        struct ProfilerBlockData
        {
            RingBuffer<ProfilerBlockTime> block_times;  //!< A ring buffer containing all block times for this type of PerformanceProfiler::ProfilerBlockData.
            double average_block_time;                  //!< The average block time for this block data.
            double last_recalculation;                  //!< The last time when the values in this block data got recalculated (average & bounds).
            double best_block_time;                     //!< The best block time (i.e. the one block time that was quickest at getting executed) that is currently in the block_times buffer.
            double best_block_time_overall;             //!< The best block time (i.e. the one block time that was quickest at getting executed) over the entire lifetime of this application.
            double worst_block_time;                    //!< The worst block time (i.e. the one block time that was slowest at getting executed) that is currently in the block_times buffer.
            double worst_block_time_overall;            //!< The worst block time (i.e. the one block time that was slowest at getting executed) over the entire lifetime of this application.
        };

        /**
        * @brief Adds a PerformanceProfiler::ProfilerBlock to the profiler. The profiler will automatically figure out how to handle it.
        * @param[in] profiler_block The block that needs to be added.
        */
        void AddProfilerBlock(ProfilerBlock& profiler_block);

        /**
        * @brief Converts a ProfilerBlockType to a String.
        * @param[in] type The type that should be converted.
        */
        String ConvertBlockTypeToString(ProfilerBlockType type);

    private:
        float last_bounds_reset_time_;                                                      //!< The last time that bounds got reset in the profiler.
        float bounds_reset_interval_;                                                       //!< The interval at which bounds should be reset.
        int history_sample_count_;                                                          //!< The number of samples that should be kept as history for all history buffers.

    private:
        bool show_window_;                                                                  //!< Whether the profiler window should be shown.

        bool catch_frame_;                                                                  //!< Whether all ProfilerBlocks in the current frames are being caught for frame analysis.
        bool catch_next_frame_;                                                             //!< Whether the next frame should be made ready for data collection.
        Vector<ProfilerBlock> collected_profiler_blocks_single_frame_;                      //!< All PerformanceProfiler::ProfilerBlocks that are collected when catch_frame_ is on, are stored in this array.
        Map<String, ProfilerBlockFrameData> condensed_profiler_block_times_;                //!< This map helps in removing duplicates from the collected_profiler_blocks_single_frame_ array.
        Vector<ProfilerBlockFrameData> profiler_blocks_single_frame_;                       //!< An array of ProfilerBlockFrameData's that spans an entire frame.

        Map<String, ProfilerBlockData> profiler_blocks_[ProfilerBlockType_COUNT];           //!< This is an array of maps that's used to categorize all the different PerformanceProfiler::ProfilerBlocks by storing their data in a map. 
        float contiguous_block_times[BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT];            //!< An array that gets re-used for every type of block time that needs to be stored contiguously (requirement for ImGui::PlotHistogram())

        ImGuiTextFilter profiler_block_filters_[ProfilerBlockType_COUNT];                   //!< An array of text filters for filtering out profiler blocks from the individual ProfilerBlock views.
    };
}