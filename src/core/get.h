#pragma once

#include "core/eastl.h"
#include <EASTL/shared_ptr.h>

/**
* @namespace blowbox
* @author Riko Ophorst
* @brief The namespace in which all blowbox symbols exist
*/
namespace blowbox
{
    class GLFWManager;
    class BlowboxCore;
    class Window;
    class ForwardRenderer;
    class DeferredRenderer;
    class CommandManager;
    class CommandContextManager;
    class Device;
    class SwapChain;
    class DescriptorHeap;
    class ImGuiManager;
    class SceneManager;

    /**
    * @class blowbox::Get
    * @author Riko Ophorst
    * @brief This class is meant to provide simple access to all managers in engine
    */
    class Get
    {
        friend class BlowboxCore;
    protected:
        /**
        * @brief Constructs the Get class. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        */
        Get();

        /**
        * @brief Destructs the Get class. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        */
        ~Get();

        /**
        * @brief Checks if all systems are set.
        * @author Riko Ophorst
        */
        void Finalize();

    public:
        /**
        * @brief Returns the BlowboxCore instance
        * @author Riko Ophorst
        */
        static BlowboxCore* BlowboxCore();

        /**
        * @brief Returns the GLFW manager instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<GLFWManager> GLFWManager();

        /**
        * @brief Returns the main Window instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<Window> MainWindow();

        /**
        * @brief Returns the ForwardRenderer instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<ForwardRenderer> ForwardRenderer();

        /**
        * @brief Returns the DeferredRenderer instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<DeferredRenderer> DeferredRenderer();

        /**
        * @brief Returns the CommandContextManager instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<CommandContextManager> CommandContextManager();

        /**
        * @brief Returns the CommandManager instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<CommandManager> CommandManager();

        /**
        * @brief Returns the Device instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<Device> Device();

        /**
        * @brief Returns the DescriptorHeap instance for render target views
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<DescriptorHeap> RtvHeap();

        /**
        * @brief Returns the DescriptorHeap instance for depth stencil views
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<DescriptorHeap> DsvHeap();

        /**
        * @brief Returns the DescriptorHeap instance for cbv/srv/uavs
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<DescriptorHeap> CbvSrvUavHeap();

        /**
        * @brief Returns the SwapChain instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<SwapChain> SwapChain();

        /**
        * @brief Returns the ImGuiManager instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<ImGuiManager> ImGuiManager();

        /**
        * @brief Returns the SceneManager instance
        * @author Riko Ophorst
        */
        static eastl::shared_ptr<SceneManager> SceneManager();
        
    protected:
        /**
        * @brief Sets the BlowboxCore instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] blowbox_core   The instance of BlowboxCore
        */
        void SetBlowboxCore(blowbox::BlowboxCore* blowbox_core);

        /**
        * @brief Sets the GLFWManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] glfw_manager   The instance of GLFWManager
        */
        void SetGLFWManager(eastl::shared_ptr<blowbox::GLFWManager> glfw_manager);

        /**
        * @brief Sets the main Window instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] main_window   The instance of Window
        */
        void SetMainWindow(eastl::shared_ptr<blowbox::Window> main_window);

        /**
        * @brief Sets the ForwardRenderer instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] forward_renderer       The instance of ForwardRenderer
        */
        void SetForwardRenderer(eastl::shared_ptr<blowbox::ForwardRenderer> forward_renderer);

        /**
        * @brief Sets the DeferredRenderer instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] deferred_renderer       The instance of DeferredRenderer
        */
        void SetDeferredRenderer(eastl::shared_ptr<blowbox::DeferredRenderer> deferred_renderer);

        /**
        * @brief Sets the CommandContextManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] command_context_manager       The instance of CommandContextManager
        */
        void SetCommandContextManager(eastl::shared_ptr<blowbox::CommandContextManager> command_context_manager);

        /**
        * @brief Sets the CommandManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] command_manager       The instance of CommandManager
        */
        void SetCommandManager(eastl::shared_ptr<blowbox::CommandManager> command_manager);

        /**
        * @brief Sets the Device instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] device       The instance of Device
        */
        void SetDevice(eastl::shared_ptr<blowbox::Device> device);

        /**
        * @brief Sets the DescriptorHeap instance for render target views. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] heap       The instance of DescriptorHeap for render target views
        */
        void SetRtvHeap(eastl::shared_ptr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the DescriptorHeap instance for depth stencil views. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] heap       The instance of DescriptorHeap for depth stencil views
        */
        void SetDsvHeap(eastl::shared_ptr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the DescriptorHeap instance for cbv/srv/uavs. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] heap       The instance of DescriptorHeap for cbv/srv/uavs
        */
        void SetCbvSrvUavHeap(eastl::shared_ptr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the SwapChain instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] swap_chain       The instance of the SwapChain
        */
        void SetSwapChain(eastl::shared_ptr<blowbox::SwapChain> swap_chain);

        /**
        * @brief Sets the ImGuiManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] imgui_manager       The instance of the ImGuiManager
        */
        void SetImGuiManager(eastl::shared_ptr<blowbox::ImGuiManager> imgui_manager);

        /**
        * @brief Sets the SceneManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] scene_manager       The instance of the scene_manager
        */
        void SetSceneManager(eastl::shared_ptr<blowbox::SceneManager> scene_manager);

        static Get* instance_; //!< The instance of the Get class

    private:
        bool finalized_; //!< Whether the Get system has been finalized
        blowbox::BlowboxCore* blowbox_core_; //!< The BlowboxCore instance
        eastl::shared_ptr<blowbox::GLFWManager> glfw_manager_; //!< The GLFWManager instance
        eastl::shared_ptr<blowbox::Window> main_window_; //!< The main Window instance
        eastl::shared_ptr<blowbox::ForwardRenderer> forward_renderer_; //!< The ForwardRenderer that is used to render stuff to the main Window
        eastl::shared_ptr<blowbox::DeferredRenderer> deferred_renderer_; //!< The DeferredRenderer that is used to render stuff to the main Window
        eastl::shared_ptr<blowbox::CommandContextManager> command_context_manager_; //!< The CommandContextManager instance
        eastl::shared_ptr<blowbox::CommandManager> command_manager_; //!< The CommandManager instance
        eastl::shared_ptr<blowbox::Device> device_; //!< The render Device instance
        eastl::shared_ptr<blowbox::DescriptorHeap> rtv_descriptor_heap_; //!< DescriptorHeap for rtvs
        eastl::shared_ptr<blowbox::DescriptorHeap> dsv_descriptor_heap_; //!< DescriptorHeap for dsvs
        eastl::shared_ptr<blowbox::DescriptorHeap> cbv_srv_uav_descriptor_heap_; //!< DescriptorHeap for cbv/srv/uavs
        eastl::shared_ptr<blowbox::SwapChain> swap_chain_; //!< The SwapChain instance
        eastl::shared_ptr<blowbox::ImGuiManager> imgui_manager_; //!< The ImGuiManager instance
        eastl::shared_ptr<blowbox::SceneManager> scene_manager_; //!< The SceneManager instance
    };
}