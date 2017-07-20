#include "frame_stats.h"

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
    FrameStats::FrameStats() :
        last_bounds_reset_time_(0.0f),
        bounds_reset_interval_(0.5f),
        history_sample_count_(200),

        show_window_(false),
        delta_time_history_(200),
        delta_times_upper_bound_(-D3D12_FLOAT32_MAX),
        delta_times_lower_bound_(D3D12_FLOAT32_MAX),
        average_delta_time_(60.0f),

        view_frame_stats_as_fps_(1)
    {

    }

    //------------------------------------------------------------------------------------------------------
    FrameStats::~FrameStats()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void FrameStats::RenderMenu()
    {
        if (ImGui::BeginMenu("Frame Stats"))
        {
            if (!show_window_)
            {
                if (ImGui::MenuItem("Show Frame Stats", "CTRL+2", false, !show_window_))
                {
                    show_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Frame Stats", "CTRL+2", false, show_window_))
                {
                    show_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_2))
        {
            show_window_ = !show_window_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void FrameStats::RenderWindow()
    {
        if (show_window_)
        {
            ImGui::SetNextWindowCollapsed(false, ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);

            if (ImGui::Begin("General Stats", &show_window_, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
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
    void FrameStats::NewFrame()
    {
        // Update histories
        {
            // delta time update
            delta_time_history_.push_back(Time::GetDeltaTime());
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

                last_bounds_reset_time_ = Time::GetProcessTime();
            }
        }

        // Update history sample count
        {
            if (delta_time_history_.capacity() != history_sample_count_)
            {
                delta_time_history_.set_capacity(history_sample_count_);
            }
        }
    }
}