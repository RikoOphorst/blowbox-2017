#include "glfw_manager.h"

#include "core/get.h"
#include "win32/window.h"

#include <GLFW/glfw3.h>

#include "core/debug/performance_profiler.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    GLFWManager::GLFWManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    GLFWManager::~GLFWManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void GLFWManager::Init()
    {
        glfwInit();
    }

    //------------------------------------------------------------------------------------------------------
    void GLFWManager::Update()
    {        
        for (auto it = windows_.begin(); it != windows_.end(); it++)
        {
            it->second->GetKeyboardState().ResetKeys();
            it->second->GetMouseState().Update();
        }

        PerformanceProfiler::ProfilerBlock block("GLFW Poll Events", ProfilerBlockType_CORE);
        glfwPollEvents();
    }

    //------------------------------------------------------------------------------------------------------
    void GLFWManager::Shutdown()
    {
        glfwTerminate();
    }
    
    //------------------------------------------------------------------------------------------------------
    void GLFWManager::AddWindow(Window* window)
    {
        windows_[window->GetGLFWWindow()] = window;
    }
    
    //------------------------------------------------------------------------------------------------------
    void GLFWManager::RemoveWindow(Window* window)
    {
        windows_[window->GetGLFWWindow()] = nullptr;
        windows_.erase(window->GetGLFWWindow());
    }
    
    //------------------------------------------------------------------------------------------------------
    Window* GLFWManager::FindCorrespondingWindow(GLFWwindow* window)
    {
        return windows_[window];
    }
}