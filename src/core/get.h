#pragma once

#include "util/shared_ptr.h"
#include "util/weak_ptr.h"

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
        static SharedPtr<GLFWManager> GLFWManager();

        /**
        * @brief Returns the main Window instance
        * @author Riko Ophorst
        */
        static SharedPtr<Window> MainWindow();

        /**
        * @brief Returns the ForwardRenderer instance
        * @author Riko Ophorst
        */
        static SharedPtr<ForwardRenderer> ForwardRenderer();

        /**
        * @brief Returns the DeferredRenderer instance
        * @author Riko Ophorst
        */
        static SharedPtr<DeferredRenderer> DeferredRenderer();

        /**
        * @brief Returns the CommandContextManager instance
        * @author Riko Ophorst
        */
        static SharedPtr<CommandContextManager> CommandContextManager();

        /**
        * @brief Returns the CommandManager instance
        * @author Riko Ophorst
        */
        static SharedPtr<CommandManager> CommandManager();

        /**
        * @brief Returns the Device instance
        * @author Riko Ophorst
        */
        static SharedPtr<Device> Device();

        /**
        * @brief Returns the DescriptorHeap instance for render target views
        * @author Riko Ophorst
        */
        static SharedPtr<DescriptorHeap> RtvHeap();

        /**
        * @brief Returns the DescriptorHeap instance for depth stencil views
        * @author Riko Ophorst
        */
        static SharedPtr<DescriptorHeap> DsvHeap();

        /**
        * @brief Returns the DescriptorHeap instance for cbv/srv/uavs
        * @author Riko Ophorst
        */
        static SharedPtr<DescriptorHeap> CbvSrvUavHeap();

        /**
        * @brief Returns the SwapChain instance
        * @author Riko Ophorst
        */
        static SharedPtr<SwapChain> SwapChain();

        /**
        * @brief Returns the ImGuiManager instance
        * @author Riko Ophorst
        */
        static SharedPtr<ImGuiManager> ImGuiManager();

        /**
        * @brief Returns the SceneManager instance
        * @author Riko Ophorst
        */
        static SharedPtr<SceneManager> SceneManager();
        
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
        void SetGLFWManager(SharedPtr<blowbox::GLFWManager> glfw_manager);

        /**
        * @brief Sets the main Window instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] main_window   The instance of Window
        */
        void SetMainWindow(SharedPtr<blowbox::Window> main_window);

        /**
        * @brief Sets the ForwardRenderer instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] forward_renderer       The instance of ForwardRenderer
        */
        void SetForwardRenderer(SharedPtr<blowbox::ForwardRenderer> forward_renderer);

        /**
        * @brief Sets the DeferredRenderer instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] deferred_renderer       The instance of DeferredRenderer
        */
        void SetDeferredRenderer(SharedPtr<blowbox::DeferredRenderer> deferred_renderer);

        /**
        * @brief Sets the CommandContextManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] command_context_manager       The instance of CommandContextManager
        */
        void SetCommandContextManager(SharedPtr<blowbox::CommandContextManager> command_context_manager);

        /**
        * @brief Sets the CommandManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] command_manager       The instance of CommandManager
        */
        void SetCommandManager(SharedPtr<blowbox::CommandManager> command_manager);

        /**
        * @brief Sets the Device instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] device       The instance of Device
        */
        void SetDevice(SharedPtr<blowbox::Device> device);

        /**
        * @brief Sets the DescriptorHeap instance for render target views. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] heap       The instance of DescriptorHeap for render target views
        */
        void SetRtvHeap(SharedPtr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the DescriptorHeap instance for depth stencil views. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] heap       The instance of DescriptorHeap for depth stencil views
        */
        void SetDsvHeap(SharedPtr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the DescriptorHeap instance for cbv/srv/uavs. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] heap       The instance of DescriptorHeap for cbv/srv/uavs
        */
        void SetCbvSrvUavHeap(SharedPtr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the SwapChain instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] swap_chain       The instance of the SwapChain
        */
        void SetSwapChain(SharedPtr<blowbox::SwapChain> swap_chain);

        /**
        * @brief Sets the ImGuiManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] imgui_manager       The instance of the ImGuiManager
        */
        void SetImGuiManager(SharedPtr<blowbox::ImGuiManager> imgui_manager);

        /**
        * @brief Sets the SceneManager instance. Only accessible to BlowboxCore.
        * @author Riko Ophorst
        * @param[in] scene_manager       The instance of the scene_manager
        */
        void SetSceneManager(SharedPtr<blowbox::SceneManager> scene_manager);

        static Get* instance_; //!< The instance of the Get class

    private:
        bool finalized_; //!< Whether the Get system has been finalized
        blowbox::BlowboxCore* blowbox_core_; //!< The BlowboxCore instance
        WeakPtr<blowbox::GLFWManager> glfw_manager_; //!< The GLFWManager instance
        WeakPtr<blowbox::Window> main_window_; //!< The main Window instance
        WeakPtr<blowbox::ForwardRenderer> forward_renderer_; //!< The ForwardRenderer that is used to render stuff to the main Window
        WeakPtr<blowbox::DeferredRenderer> deferred_renderer_; //!< The DeferredRenderer that is used to render stuff to the main Window
        WeakPtr<blowbox::CommandContextManager> command_context_manager_; //!< The CommandContextManager instance
        WeakPtr<blowbox::CommandManager> command_manager_; //!< The CommandManager instance
        WeakPtr<blowbox::Device> device_; //!< The render Device instance
        WeakPtr<blowbox::DescriptorHeap> rtv_descriptor_heap_; //!< DescriptorHeap for rtvs
        WeakPtr<blowbox::DescriptorHeap> dsv_descriptor_heap_; //!< DescriptorHeap for dsvs
        WeakPtr<blowbox::DescriptorHeap> cbv_srv_uav_descriptor_heap_; //!< DescriptorHeap for cbv/srv/uavs
        WeakPtr<blowbox::SwapChain> swap_chain_; //!< The SwapChain instance
        WeakPtr<blowbox::ImGuiManager> imgui_manager_; //!< The ImGuiManager instance
        WeakPtr<blowbox::SceneManager> scene_manager_; //!< The SceneManager instance
    };
}