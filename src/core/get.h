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
    class DebugMenu;
    class Console;
    class Time;
    class PerformanceProfiler;
    class MemoryProfiler;
    class ImageManager;
    class FileManager;
    class TextureManager;
    class MaterialManager;

    /**
    * The Get class is essentially a set of getters. It allows
    * you to access all the subsystems in the entire engine via
    * a very simple interface that just works. The Get class
    * itself stores only WeakPtr's of the subsystems, so it
    * doesn't own any subsystems - it only caches them. Upon
    * request of a subsystem, the WeakPtr is converted into a
    * SharedPtr that is provided to you.
    * 
    * @brief Provides clean access to all subsystems
    * @remarks Do NOT store the SharedPtr's that are returned
    *          by the getters - instead, convert them first to
    *          a WeakPtr. But preferably, just don't store them.
    *          Issues might arise with subsystem shutdown if
    *          you store the subsystems yourself. Be warned.
    */
    class Get
    {
        friend class BlowboxCore;
    protected:
        /** 
        * @brief Constructs the Get class. 
        * @remarks Only accessible to BlowboxCore. 
        */
        Get();

        /**
        * @brief Destructs the Get class.
        * @remarks Only accessible to BlowboxCore.
        */
        ~Get();

        /** @brief Checks if all systems are set. If any are missing, it will assert. */
        void Finalize();

    public:
        /** @returns The BlowboxCore instance */
        static BlowboxCore* BlowboxCore();

        /** @returns The GLFWManager instance. */
        static SharedPtr<GLFWManager> GLFWManager();

        /** @returns The main Window instance. */
        static SharedPtr<Window> MainWindow();

        /** @returns The ForwardRenderer instance. */
        static SharedPtr<ForwardRenderer> ForwardRenderer();

        /** @returns The DeferredRenderer instance. */
        static SharedPtr<DeferredRenderer> DeferredRenderer();

        /** @returns The CommandContextManager instance. */
        static SharedPtr<CommandContextManager> CommandContextManager();

        /** @returns The CommandManager instance. */
        static SharedPtr<CommandManager> CommandManager();

        /** @returns The Device instance. */
        static SharedPtr<Device> Device();

        /** @returns The DescriptorHeap instance for render target views. */
        static SharedPtr<DescriptorHeap> RtvHeap();

        /** @returns The DescriptorHeap instance for depth stencil views. */
        static SharedPtr<DescriptorHeap> DsvHeap();

        /** @returns The DescriptorHeap instance for cbv/srv/uavs. */
        static SharedPtr<DescriptorHeap> CbvSrvUavHeap();

        /** @returns The SwapChain instance. */
        static SharedPtr<SwapChain> SwapChain();

        /** @returns The ImGuiManager instance. */
        static SharedPtr<ImGuiManager> ImGuiManager();

        /** @returns The SceneManager instance. */
        static SharedPtr<SceneManager> SceneManager();

        /** @returns The DebugMenu instance. */
        static SharedPtr<DebugMenu> DebugMenu();

        /** @returns The Console instance. */
        static SharedPtr<Console> Console();

        /** @returns The Time instance. */
        static SharedPtr<Time> Time();

        /** @returns The PerformanceProfiler instance. */
        static SharedPtr<PerformanceProfiler> PerformanceProfiler();

        /** @returns The MemoryProfiler instance. */
        static SharedPtr<MemoryProfiler> MemoryProfiler();

        /** @returns The ImageManager instance. */
        static SharedPtr<ImageManager> ImageManager();

        /** @returns The FileManager instance. */
        static SharedPtr<FileManager> FileManager();

        /** @returns The TextureManager instance. */
        static SharedPtr<TextureManager> TextureManager();

        /** @returns The MaterialManager instance. */
        static SharedPtr<MaterialManager> MaterialManager();
        
    protected:
        /**
        * @brief Sets the BlowboxCore instance.
        * @param[in] blowbox_core   The instance of BlowboxCore.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(blowbox::BlowboxCore* blowbox_core);

        /**
        * @brief Sets the GLFWManager instance.
        * @param[in] glfw_manager   The instance of GLFWManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::GLFWManager> glfw_manager);

        /**
        * @brief Sets the main Window instance.
        * @param[in] main_window   The instance of Window.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::Window> main_window);

        /**
        * @brief Sets the ForwardRenderer instance.
        * @param[in] forward_renderer       The instance of ForwardRenderer.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::ForwardRenderer> forward_renderer);

        /**
        * @brief Sets the DeferredRenderer instance.
        * @param[in] deferred_renderer       The instance of DeferredRenderer.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::DeferredRenderer> deferred_renderer);

        /**
        * @brief Sets the CommandContextManager instance.
        * @param[in] command_context_manager       The instance of CommandContextManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::CommandContextManager> command_context_manager);

        /**
        * @brief Sets the CommandManager instance.
        * @param[in] command_manager       The instance of CommandManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::CommandManager> command_manager);

        /**
        * @brief Sets the Device instance.
        * @param[in] device       The instance of Device.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::Device> device);

        /**
        * @brief Sets the DescriptorHeap instance for render target views.
        * @param[in] heap       The instance of DescriptorHeap for render target views.
        * @remarks Only accessible to BlowboxCore.
        */
        void SetRtvHeap(SharedPtr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the DescriptorHeap instance for depth stencil views.
        * @param[in] heap       The instance of DescriptorHeap for depth stencil views.
        * @remarks Only accessible to BlowboxCore.
        */
        void SetDsvHeap(SharedPtr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the DescriptorHeap instance for cbv/srv/uavs.
        * @param[in] heap       The instance of DescriptorHeap for cbv/srv/uavs.
        * @remarks Only accessible to BlowboxCore.
        */
        void SetCbvSrvUavHeap(SharedPtr<blowbox::DescriptorHeap> heap);

        /**
        * @brief Sets the SwapChain instance.
        * @param[in] swap_chain       The instance of the SwapChain.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::SwapChain> swap_chain);

        /**
        * @brief Sets the ImGuiManager instance.
        * @param[in] imgui_manager       The instance of the ImGuiManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::ImGuiManager> imgui_manager);

        /**
        * @brief Sets the SceneManager instance.
        * @param[in] scene_manager       The instance of the SceneManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::SceneManager> scene_manager);

        /**
        * @brief Sets the DebugMenu instance.
        * @param[in] debug_menu       The instance of the DebugMenu.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::DebugMenu> debug_menu);

        /**
        * @brief Sets the Console instance.
        * @param[in] console The instance of the Console.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::Console> console);

        /**
        * @brief Sets the Time instance.
        * @param[in] time The instance of the Time.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::Time> time);

        /**
        * @brief Sets the PerformanceProfiler instance.
        * @param[in] instance The instance of the PerformanceProfiler.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::PerformanceProfiler> instance);

        /**
        * @brief Sets the MemoryProfiler instance.
        * @param[in] instance The instance of the MemoryProfiler.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::MemoryProfiler> instance);
        /**
        * @brief Sets the ImageManager instance.
        * @param[in] instance The instance of the ImageManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::ImageManager> instance);

        /**
        * @brief Sets the FileManager instance.
        * @param[in] instance The instance of the FileManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::FileManager> instance);

        /**
        * @brief Sets the TextureManager instance.
        * @param[in] instance The instance of the TextureManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::TextureManager> instance);

        /**
        * @brief Sets the MaterialManager instance.
        * @param[in] instance The instance of the MaterialManager.
        * @remarks Only accessible to BlowboxCore.
        */
        void Set(SharedPtr<blowbox::MaterialManager> instance);

        static Get* instance_;                                              //!< The instance of the Get class.

    private:
        bool finalized_;                                                    //!< Whether the Get system has been finalized.
        blowbox::BlowboxCore* blowbox_core_;                                //!< The BlowboxCore instance.
        WeakPtr<blowbox::GLFWManager> glfw_manager_;                        //!< The GLFWManager instance.
        WeakPtr<blowbox::Window> main_window_;                              //!< The main Window instance.
        WeakPtr<blowbox::ForwardRenderer> forward_renderer_;                //!< The ForwardRenderer that is used to render stuff to the main Window.
        WeakPtr<blowbox::DeferredRenderer> deferred_renderer_;              //!< The DeferredRenderer that is used to render stuff to the main Window.
        WeakPtr<blowbox::CommandContextManager> command_context_manager_;   //!< The CommandContextManager instance.
        WeakPtr<blowbox::CommandManager> command_manager_;                  //!< The CommandManager instance.
        WeakPtr<blowbox::Device> device_;                                   //!< The render Device instance.
        WeakPtr<blowbox::DescriptorHeap> rtv_descriptor_heap_;              //!< DescriptorHeap for rtvs.
        WeakPtr<blowbox::DescriptorHeap> dsv_descriptor_heap_;              //!< DescriptorHeap for dsvs.
        WeakPtr<blowbox::DescriptorHeap> cbv_srv_uav_descriptor_heap_;      //!< DescriptorHeap for cbv/srv/uavs.
        WeakPtr<blowbox::SwapChain> swap_chain_;                            //!< The SwapChain instance.
        WeakPtr<blowbox::ImGuiManager> imgui_manager_;                      //!< The ImGuiManager instance.
        WeakPtr<blowbox::SceneManager> scene_manager_;                      //!< The SceneManager instance.
        WeakPtr<blowbox::DebugMenu> debug_menu_;                            //!< The DebugMenu instance.
        WeakPtr<blowbox::Console> console_;                                 //!< The Console instance.
        WeakPtr<blowbox::Time> time_;                                       //!< The Time instance.
        WeakPtr<blowbox::PerformanceProfiler> performance_profiler_;        //!< The PerformanceProfiler instance.
        WeakPtr<blowbox::MemoryProfiler> memory_profiler_;                  //!< The MemoryProfiler instance.
        WeakPtr<blowbox::ImageManager> image_manager_;                      //!< The ImageManager instance.
        WeakPtr<blowbox::FileManager> file_manager_;                        //!< The FileManager instance.
        WeakPtr<blowbox::TextureManager> texture_manager_;                  //!< The TextureManager instance.
        WeakPtr<blowbox::MaterialManager> material_manager_;                //!< The MaterialManager instance.
    };
}