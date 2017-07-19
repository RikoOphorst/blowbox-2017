#include "profiler.h"

#include "renderer/imgui/imgui_manager.h"
#include "win32/time.h"
#include "util/vector.h"
#include "util/numeric_limits.h"
#include "core/debug/console.h"
#include <EASTL/numeric_limits.h>
#include "util/sort.h"

#include <Psapi.h>

#undef max

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Profiler::Profiler() :
        last_bounds_reset_time_(0.0f),
        bounds_reset_interval_(0.5f),
        history_sample_count_(200),

        show_general_stats_window_(false),
        delta_time_history_(200),
        delta_times_upper_bound_(-D3D12_FLOAT32_MAX),
        delta_times_lower_bound_(D3D12_FLOAT32_MAX),
        average_delta_time_(60.0f),

        view_frame_stats_as_fps_(1),

        show_performance_profiler_window_(false),
        catch_frame_(false),

        show_memory_usage_window_(false),
        ram_usage_history_(200),
        ram_usage_history_upper_bound_(0),
        ram_usage_history_lower_bound_(~((uint64_t)0)),
        vram_usage_history_(200),
        vram_usage_history_upper_bound_(0),
        vram_usage_history_lower_bound_(~((uint64_t)0))
    {

    }

    //------------------------------------------------------------------------------------------------------
    Profiler::~Profiler()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Profiler::RenderMenu()
    {
        if (ImGui::BeginMenu("Profiler"))
        {
            if (!show_general_stats_window_)
            {
                if (ImGui::MenuItem("Show General Stats", "CTRL+2", false, !show_general_stats_window_))
                {
                    show_general_stats_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide General Stats", "CTRL+2", false, show_general_stats_window_))
                {
                    show_general_stats_window_ = false;
                }
            }

            if (!show_performance_profiler_window_)
            {
                if (ImGui::MenuItem("Show Performance Profiler", "CTRL+3", false, !show_performance_profiler_window_))
                {
                    show_performance_profiler_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Performance Profiler", "CTRL+3", false, show_performance_profiler_window_))
                {
                    show_performance_profiler_window_ = false;
                }
            }

            if (!show_memory_usage_window_)
            {
                if (ImGui::MenuItem("Show Memory Usage", "CTRL+4", false, !show_memory_usage_window_))
                {
                    show_memory_usage_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Memory Usage", "CTRL+4", false, show_memory_usage_window_))
                {
                    show_memory_usage_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_2))
        {
            show_general_stats_window_ = !show_general_stats_window_;
        }

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_3))
        {
            show_performance_profiler_window_ = !show_performance_profiler_window_;
        }

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_4))
        {
            show_memory_usage_window_ = !show_memory_usage_window_;
        }
    }

    struct CompareProfilerBlockSingleFrame
    {
        inline bool operator() (const Profiler::ProfilerBlockFrameData& a, const Profiler::ProfilerBlockFrameData& b)
        {
            return (a.total_time > b.total_time);
        }
    };

    //------------------------------------------------------------------------------------------------------
    void Profiler::RenderWindow()
    {
        if (show_general_stats_window_)
        {
            ImGui::SetNextWindowCollapsed(false, ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);

            if (ImGui::Begin("General Stats", &show_general_stats_window_, ImGuiWindowFlags_NoResize))
            {
                if (view_frame_stats_as_fps_ == 0)
                {
                    int delta_times_count = (delta_time_history_.size() < BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT ? delta_time_history_.size() : BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT);

                    for (int i = 0; i < delta_times_count; i++)
                    {
                        delta_time_history_contiguous_[i] = delta_time_history_[i];
                    }

                    String current_delta_time = eastl::to_string(Time::GetDeltaTime() * 1000.0f) + " ms";

                    ImGui::PlotHistogram(current_delta_time.c_str(), delta_time_history_contiguous_, delta_times_count, 0, nullptr, 0.0f, 0.04f, ImVec2(300, 50), 4);

                    ImGui::Columns(2, nullptr, false);
                    ImGui::Text("Best delta time:"); 
                    ImGui::NextColumn();
                    ImGui::Text("%g ms", delta_times_lower_bound_ * 1000.0f);
                    ImGui::NextColumn();

                    ImGui::Text("Worst delta time:"); 
                    ImGui::NextColumn();
                    ImGui::Text("%g ms", delta_times_upper_bound_ * 1000.0f);
                    ImGui::NextColumn();

                    ImGui::Text("Average delta time:"); 
                    ImGui::NextColumn();
                    ImGui::Text("%g ms", average_delta_time_ * 1000.0f);
                    ImGui::Columns(1);
                }
                else
                {
                    int delta_times_count = (delta_time_history_.size() < BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT ? delta_time_history_.size() : BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT);

                    for (int i = 0; i < delta_times_count; i++)
                    {
                        fps_history_contiguous_[i] = 1.0f / delta_time_history_[i];
                    }

                    String current_fps = eastl::to_string(1.0f / Time::GetDeltaTime()) + " FPS";

                    ImGui::PlotHistogram(current_fps.c_str(), fps_history_contiguous_, delta_times_count, 0, nullptr, 0.0f, 200.0f, ImVec2(300, 50), 4);

                    ImGui::Columns(2, nullptr, false);
                    ImGui::Text("Best FPS:"); 
                    ImGui::NextColumn();
                    ImGui::Text("%g FPS", 1.0f / delta_times_lower_bound_);
                    ImGui::NextColumn();

                    ImGui::Text("Worst FPS:"); 
                    ImGui::NextColumn();
                    ImGui::Text("%g FPS", 1.0f / delta_times_upper_bound_);
                    ImGui::NextColumn();

                    ImGui::Text("Average FPS:"); 
                    ImGui::NextColumn();
                    ImGui::Text("%g FPS", 1.0f / average_delta_time_);
                    ImGui::Columns(1);
                }

                ImGui::Separator();

                ImGui::Columns(2);
                ImGui::RadioButton("View as Delta Time", &view_frame_stats_as_fps_, 0); 
                ImGui::NextColumn();
                ImGui::RadioButton("View as FPS", &view_frame_stats_as_fps_, 1);
                ImGui::Columns(1);

                ImGui::SliderInt("History sample count", &history_sample_count_, BLOWBOX_PROFILER_HISTORY_MIN_SAMPLE_COUNT, BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT);
                history_sample_count_ = eastl::clamp(history_sample_count_, BLOWBOX_PROFILER_HISTORY_MIN_SAMPLE_COUNT, BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT);

                ImGui::End();
            }
        }

        if (show_performance_profiler_window_)
        {
            ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
            
            if (ImGui::Begin("Performance Profiler Main", &show_performance_profiler_window_))
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

                                            if (Time::GetProcessTimeAsDouble() - block_data.last_recalculation > (double)bounds_reset_interval_)
                                            {
                                                if (current_time > block_data.worst_block_time)
                                                    block_data.worst_block_time = current_time;

                                                if (current_time < block_data.best_block_time)
                                                    block_data.best_block_time = current_time;
                                            }
                                        }

                                        if (Time::GetProcessTimeAsDouble() - block_data.last_recalculation > (double)bounds_reset_interval_)
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

        if (show_memory_usage_window_)
        {
            if (ImGui::Begin("Memory Usage", &show_performance_profiler_window_, ImGuiWindowFlags_NoResize))
            {
                int ram_usage_count = (ram_usage_history_.size() < BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT ? ram_usage_history_.size() : BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT);

                for (int i = 0; i < ram_usage_count; i++)
                {
                    ram_usage_history_contiguous_[i] = static_cast<float>(ram_usage_history_[i] / 1000);
                }

                ImGui::PlotHistogram("RAM Usage (KB)", ram_usage_history_contiguous_, ram_usage_count, 0, nullptr, 0.0f, ram_operating_system_budget_ / 1000, ImVec2(300, 50), 4);

ImGui::Columns(2, nullptr, false);
ImGui::Text("Lowest RAM usage:");
ImGui::NextColumn();
ImGui::Text("%i KB", ram_usage_history_lower_bound_ / 1000);
ImGui::NextColumn();

ImGui::Text("Highest RAM usage:");
ImGui::NextColumn();
ImGui::Text("%i KB", ram_usage_history_upper_bound_ / 1000);
ImGui::NextColumn();

ImGui::Text("Average RAM usage:");
ImGui::NextColumn();
ImGui::Text("%i KB", ram_average_usage_ / 1000);
ImGui::NextColumn();

ImGui::Text("RAM OS budget:");
ImGui::NextColumn();
ImGui::Text("%i KB (%g%% used)", ram_operating_system_budget_ / 1000, (static_cast<float>(ram_average_usage_) / static_cast<float>(ram_operating_system_budget_)) * 100.0f);
if (ImGui::IsItemHovered())
ImGui::SetTooltip("This value is based on how much RAM is globally available on this machine.");
ImGui::Columns(1);

ImGui::Separator();

int vram_usage_count = (vram_usage_history_.size() < BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT ? vram_usage_history_.size() : BLOWBOX_PROFILER_HISTORY_MAX_SAMPLE_COUNT);

for (int i = 0; i < vram_usage_count; i++)
{
    vram_usage_history_contiguous_[i] = static_cast<float>(vram_usage_history_[i] / 1000);
}

ImGui::PlotHistogram("VRAM Usage (MB)", vram_usage_history_contiguous_, vram_usage_count, 0, nullptr, 0.0f, vram_operating_system_budget_ / 1000, ImVec2(300, 50), 4);

ImGui::Columns(2, nullptr, false);
ImGui::Text("Lowest VRAM usage:");
ImGui::NextColumn();
ImGui::Text("%i KB", vram_usage_history_lower_bound_ / 1000);
ImGui::NextColumn();

ImGui::Text("Highest VRAM usage:");
ImGui::NextColumn();
ImGui::Text("%i KB", vram_usage_history_upper_bound_ / 1000);
ImGui::NextColumn();

ImGui::Text("Average VRAM usage:");
ImGui::NextColumn();
ImGui::Text("%i KB", vram_average_usage_ / 1000);
ImGui::NextColumn();

ImGui::Text("VRAM OS budget:");
ImGui::NextColumn();
ImGui::Text("%i KB (%g%% used)", vram_operating_system_budget_ / 1000, (static_cast<float>(vram_average_usage_) / static_cast<float>(vram_operating_system_budget_)) * 100.0f);
ImGui::Columns(1);

ImGui::End();
            }
        }

        ImGui::Begin(
            "FPSDisplay",
            nullptr,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoFocusOnAppearing);
        ImGui::Text("FPS: %g", 1.0f / average_delta_time_);
        ImGui::End();
    }

    //------------------------------------------------------------------------------------------------------
    void Profiler::NewFrame()
    {
        if (catch_next_frame_ == true)
        {
            catch_frame_ = true;
            catch_next_frame_ = false;
        }
        else if (catch_frame_ == true)
        {
            catch_frame_ = false;

            //eastl::quick_sort<Vector<ProfilerBlock>::iterator, CompareProfilerBlocks>(profiler_blocks_per_frame_.begin(), profiler_blocks_per_frame_.end(), CompareProfilerBlocks());

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

        // Update histories
        {
            // delta time update
            delta_time_history_.push_back(Time::GetDeltaTime());

            // RAM update
            MEMORYSTATUSEX ram_stats_global;
            ram_stats_global.dwLength = sizeof(ram_stats_global);
            GlobalMemoryStatusEx(&ram_stats_global);

            PROCESS_MEMORY_COUNTERS ram_stats_process;
            ram_stats_process.cb = sizeof(ram_stats_process);
            GetProcessMemoryInfo(GetCurrentProcess(), &ram_stats_process, ram_stats_process.cb);

            ram_usage_history_.push_back(ram_stats_process.WorkingSetSize);
            ram_operating_system_budget_ = ram_stats_global.ullAvailPhys;

            // VRAM update
            DXGI_QUERY_VIDEO_MEMORY_INFO vram_stats;
            Get::Device()->GetAdapter().dxgi_adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &vram_stats);

            vram_operating_system_budget_ = vram_stats.Budget;
            vram_usage_history_.push_back(vram_stats.CurrentUsage);
        }

        // Update history bounds
        {
            if (Time::GetProcessTime() - last_bounds_reset_time_ > bounds_reset_interval_)
            {
                // Delta time update
                {
                    delta_times_upper_bound_ = (-D3D12_FLOAT32_MAX);
                    delta_times_lower_bound_ = (D3D12_FLOAT32_MAX);

                    float total_delta_time = 0.0f;
                    for (int i = 0; i < delta_time_history_.size(); i++)
                    {
                        total_delta_time += delta_time_history_[i];

                        if (delta_time_history_[i] < delta_times_lower_bound_)
                            delta_times_lower_bound_ = delta_time_history_[i];

                        if (delta_time_history_[i] > delta_times_upper_bound_)
                            delta_times_upper_bound_ = delta_time_history_[i];
                    }

                    average_delta_time_ = (total_delta_time / delta_time_history_.size());
                }

                // RAM update
                {
                    uint64_t total_ram_usage = 0;
                    for (int i = 0; i < ram_usage_history_.size(); i++)
                    {
                        total_ram_usage += ram_usage_history_[i];

                        if (ram_usage_history_[i] < ram_usage_history_lower_bound_)
                            ram_usage_history_lower_bound_ = ram_usage_history_[i];

                        if (ram_usage_history_[i] > ram_usage_history_upper_bound_)
                            ram_usage_history_upper_bound_ = ram_usage_history_[i];
                    }

                    ram_average_usage_ = static_cast<uint64_t>(static_cast<float>(total_ram_usage) / static_cast<float>(ram_usage_history_.size()));
                }

                // VRAM update
                {
                    uint64_t total_vram_usage = 0;
                    for (int i = 0; i < vram_usage_history_.size(); i++)
                    {
                        total_vram_usage += vram_usage_history_[i];

                        if (vram_usage_history_[i] < vram_usage_history_lower_bound_)
                            vram_usage_history_lower_bound_ = vram_usage_history_[i];

                        if (vram_usage_history_[i] > vram_usage_history_upper_bound_)
                            vram_usage_history_upper_bound_ = vram_usage_history_[i];
                    }

                    vram_average_usage_ = static_cast<uint64_t>(static_cast<float>(total_vram_usage) / static_cast<float>(vram_usage_history_.size()));
                }

                last_bounds_reset_time_ = Time::GetProcessTime();
            }
        }

        // Update history sample count
        {
            if (delta_time_history_.capacity() != history_sample_count_)
            {
                delta_time_history_.set_capacity(history_sample_count_);
            }

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

            if (ram_usage_history_.capacity() != history_sample_count_)
            {
                ram_usage_history_.set_capacity(history_sample_count_);
            }

            if (vram_usage_history_.capacity() != history_sample_count_)
            {
                vram_usage_history_.set_capacity(history_sample_count_);
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void Profiler::CatchNextFrame()
    {
        catch_next_frame_ = true;
        collected_profiler_blocks_single_frame_.clear();
        condensed_profiler_block_times_.clear();
        profiler_blocks_single_frame_.clear();
    }

    //------------------------------------------------------------------------------------------------------
    void Profiler::AddProfilerBlock(ProfilerBlock& profiler_block)
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
    String Profiler::ConvertBlockTypeToString(ProfilerBlockType type)
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
    Profiler::ProfilerBlock::ProfilerBlock() :
        start_time_(-1.0)
    {

    }

    //------------------------------------------------------------------------------------------------------
    Profiler::ProfilerBlock::ProfilerBlock(const String& block_name, const ProfilerBlockType& block_type) :
        block_type_(block_type),
        block_name_(block_name),
        start_time_(0.0f),
        end_time_(0.0f),
        finished_(false)
    {
        start_time_ = glfwGetTime();
    }

    //------------------------------------------------------------------------------------------------------
    Profiler::ProfilerBlock::~ProfilerBlock()
    {
        Finish();
    }

    //------------------------------------------------------------------------------------------------------
    void Profiler::ProfilerBlock::Restart()
    {
        start_time_ = glfwGetTime();
        end_time_ = 0.0f;
        finished_ = false;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Profiler::ProfilerBlock::Finish()
    {
        if (start_time_ != -1.0)
        {
            if (finished_ == false)
            {
                end_time_ = glfwGetTime();
                finished_ = true;
                Get::Profiler()->AddProfilerBlock(*this);
            }
        }
    }
}