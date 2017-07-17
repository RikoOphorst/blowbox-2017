#include "console.h"

#include "core/get.h"
#include "renderer/imgui/imgui_manager.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Console::Console() :
        show_console_window_(false),
        message_buffer_(250),
        new_message_added_(false),
        auto_scroll_(true)
    {

    }

    //------------------------------------------------------------------------------------------------------
    Console::~Console()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Console::RenderMenu()
    {
        if (ImGui::BeginMenu("Console"))
        {
            if (!show_console_window_)
            {
                if (ImGui::MenuItem("Show", "CTRL+1", false, !show_console_window_))
                {
                    show_console_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide", "CTRL+1", false, show_console_window_))
                {
                    show_console_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_1))
        {
            show_console_window_ = !show_console_window_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void Console::LogStatus(const String& message)
    {
        Message log;
        log.message = message;
        log.color = DirectX::XMFLOAT4(
            255.0f  / 255.0f, 
            255.0f  / 255.0f, 
            255.0f  / 255.0f, 
            255.0f  / 255.0f
        );

        AddMessage(log);
    }

    //------------------------------------------------------------------------------------------------------
    void Console::LogWarning(const String& message)
    {
        Message log;
        log.message = message;
        log.color = DirectX::XMFLOAT4(
            236.0f  / 255.0f, 
            153.0f  / 255.0f, 
            11.0f   / 255.0f, 
            255.0f  / 255.0f
        );

        AddMessage(log);
    }

    //------------------------------------------------------------------------------------------------------
    void Console::LogError(const String& message)
    {
        Message log;
        log.message = message;
        log.color = DirectX::XMFLOAT4(
            189.0f  / 255.0f,
            21.0f   / 255.0f,
            34.0f   / 255.0f,
            255.0f  / 255.0f
        );

        AddMessage(log);
    }

    //------------------------------------------------------------------------------------------------------
    void Console::Log(const String& message, const DirectX::XMFLOAT4& color)
    {
        Message log;
        log.message = message;
        log.color = color;

        AddMessage(log);
    }

    //------------------------------------------------------------------------------------------------------
    void Console::Clear()
    {
        message_buffer_.clear();
    }

    //------------------------------------------------------------------------------------------------------
    void Console::AddMessage(Message& message)
    {
        time_t t = time(0);
        struct tm now;
        localtime_s(&now, &t);
        String hour = eastl::to_string(now.tm_hour);

        if (hour.size() < 2)
            hour = "0" + hour;

        String min = eastl::to_string(now.tm_min);
        if (min.size() < 2)
            min = "0" + min;

        String sec = eastl::to_string(now.tm_sec);
        if (sec.size() < 2)
            sec = "0" + sec;

        String time_stamp = hour + ":" + min + ":" + sec + " ";

        message.time_stamp = time_stamp;

        message_buffer_.push_back(message);
        new_message_added_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void Console::RenderWindow()
    {
        if (show_console_window_)
        {
            ImGui::SetNextWindowCollapsed(false, ImGuiSetCond_Once);
            ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiSetCond_Once);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_Once);

            if (ImGui::Begin("Console", &show_console_window_))
            {
                {
                    ImGui::BeginChild("ConsoleRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar);

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

                    for (int i = 0; i < message_buffer_.size(); i++)
                    {
                        ImGui::TextUnformatted(message_buffer_[i].time_stamp.c_str());
                        ImGui::SameLine();
                        ImGui::PushStyleColor(ImGuiCol_Text, ImColor(message_buffer_[i].color.x, message_buffer_[i].color.y, message_buffer_[i].color.z, message_buffer_[i].color.w));
                        ImGui::TextUnformatted(message_buffer_[i].message.c_str());
                        ImGui::PopStyleColor();
                    }

                    if (auto_scroll_ && new_message_added_)
                    {
                        ImGui::SetScrollHere();
                        new_message_added_ = false;
                    }

                    ImGui::PopStyleVar();

                    ImGui::EndChild();
                }

                ImGui::Separator();

                {
                    ImGui::Columns(2, nullptr, false);

                    ImGui::Checkbox("Automatically scroll to bottom", &auto_scroll_);
                    ImGui::NextColumn();

                    if (ImGui::Button("Clear the entire console"))
                    {
                        Clear();
                    }

                    ImGui::Columns(1);
                }

                ImGui::End();
            }
        }
    }
}