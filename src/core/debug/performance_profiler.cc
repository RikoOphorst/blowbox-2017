#include "performance_profiler.h"

#include "renderer/imgui/imgui_manager.h"
#include "win32/time.h"
#include "util/vector.h"
#include "util/numeric_limits.h"
#include "core/debug/console.h"
#include <EASTL/numeric_limits.h>
#include "util/sort.h"
#include "renderer/buffers/gpu_resource.h"

#include <Psapi.h>

#undef max

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    PerformanceProfiler::PerformanceProfiler() :
        last_bounds_reset_time_(0.0f),
        bounds_reset_interval_(0.5f),
        history_sample_count_(200),

        show_window_(false),
        catch_frame_(false),
        catch_next_frame_(false)
    {

    }

    //------------------------------------------------------------------------------------------------------
    PerformanceProfiler::~PerformanceProfiler()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void PerformanceProfiler::RenderMenu()
    {
        if (ImGui::BeginMenu("Performance Profiler"))
        {
            if (!show_window_)
            {
                if (ImGui::MenuItem("Show Performance Profiler", "CTRL+4", false, !show_window_))
                {
                    show_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Performance Profiler", "CTRL+4", false, show_window_))
                {
                    show_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_4))
        {
            show_window_ = !show_window_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void PerformanceProfiler::RenderWindow()
    {
        if (show_window_)
        {
            ProfilerBlock block("PerformanceProfiler::RenderWindow");
            ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);

            if (ImGui::Begin("Performance Profiler Main", &show_window_, ImGuiWindowFlags_NoCollapse))
            {
                if (ImGui::CollapsingHeader("Frame performance"))
                {
                    ImGui::Indent(ImGui::GetStyle().IndentSpacing / 2.0f);
                    ImGui::BeginGroup();
                    if (ImGui::Button("Catch a single frame"))
                    {
                        CatchNextFrame();
                    }

                    if (profiler_blocks_single_frame_.size() > 0)
                    {
                        ImGui::BeginChild("Frame Stats", ImVec2(560.0f, 300.0f), true);
                        ImGui::Columns(3);

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                        ImGui::Text("Block Category");
                        ImGui::NextColumn();
                        ImGui::Text("Block Name");
                        ImGui::NextColumn();
                        ImGui::Text("Total Time");
                        ImGui::Separator();
                        ImGui::NextColumn();
                        ImGui::PopStyleColor();

                        for (int i = 0; i < profiler_blocks_single_frame_.size(); i++)
                        {
                            ImGui::Text("%s", ConvertBlockTypeToString(profiler_blocks_single_frame_[i].block_type).c_str());
                            ImGui::NextColumn();
                            ImGui::Text("%s", profiler_blocks_single_frame_[i].block_name.c_str());
                            ImGui::NextColumn();
                            ImGui::Text("%g ms", profiler_blocks_single_frame_[i].total_time * 1000.0);
                            ImGui::NextColumn();
                        }
                        ImGui::Columns(1);
                        ImGui::EndChild();
                    }
                    else
                    {
                        ImGui::Separator();
                        ImGui::TextUnformatted("You haven't caught a frame yet. Go catch one.");
                    }

                    ImGui::EndGroup();
                    ImGui::Unindent(ImGui::GetStyle().IndentSpacing / 2.0f);
                }

                if (ImGui::CollapsingHeader("Individual block performance"))
                {
                    ImGui::Indent(ImGui::GetStyle().IndentSpacing / 2.0f);
                    for (int i = 0; i < ProfilerBlockType_COUNT; i++)
                    {
                        if (ImGui::CollapsingHeader(ConvertBlockTypeToString(static_cast<ProfilerBlockType>(i)).c_str()))
                        {
                            Map<String, ProfilerBlockData>& profiler_blocks = profiler_blocks_[i];

                            profiler_block_filters_[i].Draw("Find profiling blocks", 220);

                            for (auto it = profiler_blocks.begin(); it != profiler_blocks.end(); it++)
                            {
                                if (profiler_block_filters_[i].PassFilter(it->first.c_str()))
                                {
                                    if (ImGui::TreeNode(it->first.c_str()))
                                    {
                                        ProfilerBlockData& block_data = it->second;
                                        RingBuffer<ProfilerBlockTime>& block_times = block_data.block_times;
                                        float most_current_block_time = block_times[0].end_time - block_times[0].start_time;
                                        float total_block_time = 0.0f;

                                        if (Time::GetProcessTimeAsDouble() - block_data.last_recalculation > (double)bounds_reset_interval_)
                                        {
                                            block_data.worst_block_time = -(D3D12_FLOAT32_MAX);
                                            block_data.best_block_time = (D3D12_FLOAT32_MAX);
                                        }

                                        for (int j = 0; j < block_times.size(); j++)
                                        {
                                            double current_time = block_times[j].end_time - block_times[j].start_time;

                                            contiguous_block_times[j] = static_cast<float>(current_time);

                                            total_block_time += current_time;

                                            if (Time::GetProcessTimeAsDouble() - block_data.last_recalculation >(double)bounds_reset_interval_)
                                            {
                                                if (current_time > block_data.worst_block_time)
                                                    block_data.worst_block_time = current_time;

                                                if (current_time < block_data.best_block_time)
                                                    block_data.best_block_time = current_time;
                                            }
                                        }

                                        if (Time::GetProcessTimeAsDouble() - block_data.last_recalculation >(double)bounds_reset_interval_)
                                        {
                                            if (block_data.worst_block_time_overall < block_data.worst_block_time)
                                            {
                                                block_data.worst_block_time_overall = block_data.worst_block_time;
                                            }

                                            if (block_data.best_block_time_overall > block_data.best_block_time)
                                            {
                                                block_data.best_block_time_overall = block_data.best_block_time;
                                            }

                                            block_data.average_block_time = total_block_time / block_times.size();

                                            block_data.last_recalculation = Time::GetProcessTimeAsDouble();
                                        }

                                        ImGui::PlotHistogram("Block history", contiguous_block_times, block_times.size(), 0, nullptr, 0.0f, block_data.worst_block_time_overall * 1.1f, ImVec2(260, 50), 4);

                                        ImGui::Columns(2);
                                        ImGui::Text("Last block time:");
                                        ImGui::NextColumn();
                                        ImGui::Text("%g ms", most_current_block_time * 1000.0f);
                                        ImGui::NextColumn();
                                        ImGui::Text("Best time:");
                                        ImGui::NextColumn();
                                        ImGui::Text("%g ms", block_data.best_block_time * 1000.0f);
                                        ImGui::NextColumn();
                                        ImGui::Text("Best time overall:");
                                        ImGui::NextColumn();
                                        ImGui::Text("%g ms", block_data.best_block_time_overall * 1000.0f);
                                        ImGui::NextColumn();
                                        ImGui::Text("Worst time:");
                                        ImGui::NextColumn();
                                        ImGui::Text("%g ms", block_data.worst_block_time * 1000.0f);
                                        ImGui::NextColumn();
                                        ImGui::Text("Worst time overall:");
                                        ImGui::NextColumn();
                                        ImGui::Text("%g ms", block_data.worst_block_time_overall * 1000.0f);
                                        ImGui::NextColumn();
                                        ImGui::Text("Average time:");
                                        ImGui::NextColumn();
                                        ImGui::Text("%g ms", block_data.average_block_time * 1000.0f);
                                        ImGui::Columns(1);

                                        ImGui::TreePop();
                                    }
                                }
                            }
                        }
                    }
                    ImGui::Unindent(ImGui::GetStyle().IndentSpacing / 2.0f);
                }

                ImGui::End();
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void PerformanceProfiler::NewFrame()
    {
        ProfilerBlock block("PerformanceProfiler::NewFrame");
        if (catch_next_frame_ == true)
        {
            catch_frame_ = true;
            catch_next_frame_ = false;
        }
        else if (catch_frame_ == true)
        {
            catch_frame_ = false;

            for (int i = 0; i < collected_profiler_blocks_single_frame_.size(); i++)
            {
                ProfilerBlock& current = collected_profiler_blocks_single_frame_[i];

                if (condensed_profiler_block_times_.find(current.block_name_) == condensed_profiler_block_times_.end())
                {
                    condensed_profiler_block_times_[current.block_name_].block_name = current.block_name_;
                    condensed_profiler_block_times_[current.block_name_].block_type = current.block_type_;
                    condensed_profiler_block_times_[current.block_name_].total_time = 0.0;
                }

                condensed_profiler_block_times_[current.block_name_].total_time += current.end_time_ - current.start_time_;
            }

            for (auto it = condensed_profiler_block_times_.begin(); it != condensed_profiler_block_times_.end(); it++)
            {
                profiler_blocks_single_frame_.push_back(it->second);
            }

            eastl::quick_sort<Vector<ProfilerBlockFrameData>::iterator, CompareProfilerBlockSingleFrame>(profiler_blocks_single_frame_.begin(), profiler_blocks_single_frame_.end(), CompareProfilerBlockSingleFrame());
        }

        // Update history sample count
        {
            for (int i = 0; i < ProfilerBlockType_COUNT; i++)
            {
                for (auto it = profiler_blocks_[i].begin(); it != profiler_blocks_[i].end(); it++)
                {
                    if (it->second.block_times.capacity() != history_sample_count_)
                    {
                        it->second.block_times.set_capacity(history_sample_count_);
                    }
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void PerformanceProfiler::CatchNextFrame()
    {
        catch_next_frame_ = true;
        collected_profiler_blocks_single_frame_.clear();
        condensed_profiler_block_times_.clear();
        profiler_blocks_single_frame_.clear();
    }

    //------------------------------------------------------------------------------------------------------
    void PerformanceProfiler::AddProfilerBlock(ProfilerBlock& profiler_block)
    {
        ProfilerBlockTime profiler_block_light;
        profiler_block_light.start_time = profiler_block.start_time_;
        profiler_block_light.end_time = profiler_block.end_time_;

        if (profiler_blocks_[profiler_block.block_type_].find(profiler_block.block_name_) == profiler_blocks_[profiler_block.block_type_].end())
        {
            profiler_blocks_[profiler_block.block_type_][profiler_block.block_name_].average_block_time = 0.0;
            profiler_blocks_[profiler_block.block_type_][profiler_block.block_name_].best_block_time = 0.0;
            profiler_blocks_[profiler_block.block_type_][profiler_block.block_name_].best_block_time_overall = D3D12_FLOAT32_MAX;
            profiler_blocks_[profiler_block.block_type_][profiler_block.block_name_].worst_block_time = 0.0;
            profiler_blocks_[profiler_block.block_type_][profiler_block.block_name_].worst_block_time_overall = 0.0;
            profiler_blocks_[profiler_block.block_type_][profiler_block.block_name_].last_recalculation = 0.0;
            profiler_blocks_[profiler_block.block_type_][profiler_block.block_name_].block_times.set_capacity(history_sample_count_);
        }

        profiler_blocks_[profiler_block.block_type_][profiler_block.block_name_].block_times.push_back(profiler_block_light);

        if (catch_frame_ == true)
        {
            collected_profiler_blocks_single_frame_.push_back(profiler_block);
        }
    }

    //------------------------------------------------------------------------------------------------------
    String PerformanceProfiler::ConvertBlockTypeToString(ProfilerBlockType type)
    {
        switch (type)
        {
        case ProfilerBlockType_CONTENT:
            return "Content";
            break;
        case ProfilerBlockType_CORE:
            return "Core";
            break;
        case ProfilerBlockType_GAME:
            return "Game";
            break;
        case ProfilerBlockType_MISC:
            return "Misc";
            break;
        case ProfilerBlockType_RENDERER:
            return "Renderer";
            break;
        }

        return "Unknown";
    }

    //------------------------------------------------------------------------------------------------------
    PerformanceProfiler::ProfilerBlock::ProfilerBlock() :
        start_time_(-1.0)
    {

    }

    //------------------------------------------------------------------------------------------------------
    PerformanceProfiler::ProfilerBlock::ProfilerBlock(const String& block_name, const ProfilerBlockType& block_type) :
        block_type_(block_type),
        block_name_(block_name),
        start_time_(0.0f),
        end_time_(0.0f),
        finished_(false)
    {
        start_time_ = glfwGetTime();
    }

    //------------------------------------------------------------------------------------------------------
    PerformanceProfiler::ProfilerBlock::~ProfilerBlock()
    {
        Finish();
    }

    //------------------------------------------------------------------------------------------------------
    void PerformanceProfiler::ProfilerBlock::Restart()
    {
        start_time_ = glfwGetTime();
        end_time_ = 0.0f;
        finished_ = false;
    }

    //------------------------------------------------------------------------------------------------------
    void PerformanceProfiler::ProfilerBlock::Finish()
    {
        if (start_time_ != -1.0)
        {
            if (finished_ == false)
            {
                end_time_ = glfwGetTime();
                finished_ = true;
                Get::PerformanceProfiler()->AddProfilerBlock(*this);
            }
        }
    }
}