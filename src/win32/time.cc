#include "time.h"

#include <GLFW/glfw3.h>
#include "core/get.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Time::Time()
    {
        initialization_time_ = glfwGetTime();
        delta_time_ = 0.0;
        frame_start_time_ = 0.0;
    }
    
    //------------------------------------------------------------------------------------------------------
    Time::~Time()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Time::NewFrame()
    {
        process_time_ = glfwGetTime();
        delta_time_ = process_time_ - frame_start_time_;
        frame_start_time_ = process_time_;
    }

    //------------------------------------------------------------------------------------------------------
    float Time::GetDeltaTime()
    {
        return static_cast<float>(Get::Time()->delta_time_);
    }

    //------------------------------------------------------------------------------------------------------
    double Time::GetDeltaTimeAsDouble()
    {
        return Get::Time()->delta_time_;
    }
    
    //------------------------------------------------------------------------------------------------------
    float Time::GetProcessTime()
    {
        return static_cast<float>(Get::Time()->process_time_);
    }
    
    //------------------------------------------------------------------------------------------------------
    double Time::GetProcessTimeAsDouble()
    {
        return Get::Time()->process_time_;
    }
}