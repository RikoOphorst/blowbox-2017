#include "memory_stats.h"

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
    MemoryStats::MemoryStats() :
        last_bounds_reset_time_(0.0f),
        bounds_reset_interval_(0.5f),
        history_sample_count_(200),

        show_window_(false),
        ram_usage_history_(200),
        ram_usage_history_upper_bound_(0),
        ram_usage_history_lower_bound_(~((uint64_t)0)),
        vram_usage_history_(200),
        vram_usage_history_upper_bound_(0),
        vram_usage_history_lower_bound_(~((uint64_t)0))
    {

    }

    //------------------------------------------------------------------------------------------------------
    MemoryStats::~MemoryStats()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MemoryStats::RenderMenu()
    {
        if (ImGui::BeginMenu("Memory Stats"))
        {
            if (!show_window_)
            {
                if (ImGui::MenuItem("Show Memory Stats", "CTRL+3", false, !show_window_))
                {
                    show_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Memory Stats", "CTRL+3", false, show_window_))
                {
                    show_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_3))
        {
            show_window_ = !show_window_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryStats::RenderWindow()
    {
        if (show_window_)
        {
            if (ImGui::Begin("Memory Usage", &show_window_, ImGuiWindowFlags_NoResize))
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
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryStats::NewFrame()
    {
        // Update histories
        {
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
}