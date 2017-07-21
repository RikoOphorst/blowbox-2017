#include "get.h"

#include "util/assert.h"

namespace blowbox
{
    Get* Get::instance_ = nullptr;

    //------------------------------------------------------------------------------------------------------
    Get::Get() :
        finalized_(false),
        blowbox_core_(nullptr)
    {
        BLOWBOX_ASSERT(instance_ == nullptr);
        instance_ = this;
    }

    //------------------------------------------------------------------------------------------------------
    Get::~Get()
    {
        instance_ = nullptr;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Finalize()
    {
        BLOWBOX_ASSERT(blowbox_core_                            != nullptr);
        BLOWBOX_ASSERT(glfw_manager_.use_count()                > 0);
        BLOWBOX_ASSERT(main_window_.use_count()                 > 0);
        BLOWBOX_ASSERT(forward_renderer_.use_count()            > 0);
        BLOWBOX_ASSERT(deferred_renderer_.use_count()           > 0);
        BLOWBOX_ASSERT(command_context_manager_.use_count()     > 0);
        BLOWBOX_ASSERT(command_manager_.use_count()             > 0);
        BLOWBOX_ASSERT(device_.use_count()                      > 0);
        BLOWBOX_ASSERT(rtv_descriptor_heap_.use_count()         > 0);
        BLOWBOX_ASSERT(dsv_descriptor_heap_.use_count()         > 0);
        BLOWBOX_ASSERT(cbv_srv_uav_descriptor_heap_.use_count() > 0);
        BLOWBOX_ASSERT(swap_chain_.use_count()                  > 0);
        BLOWBOX_ASSERT(imgui_manager_.use_count()               > 0);
        BLOWBOX_ASSERT(scene_manager_.use_count()               > 0);
        BLOWBOX_ASSERT(debug_menu_.use_count()                  > 0);
        BLOWBOX_ASSERT(console_.use_count()                     > 0);
        BLOWBOX_ASSERT(time_.use_count()                        > 0);
        BLOWBOX_ASSERT(performance_profiler_.use_count()        > 0);
        BLOWBOX_ASSERT(memory_profiler_.use_count()             > 0);
        BLOWBOX_ASSERT(frame_stats_.use_count()                 > 0);
        BLOWBOX_ASSERT(memory_stats_.use_count()                > 0);
        BLOWBOX_ASSERT(image_manager_.use_count()               > 0);
        BLOWBOX_ASSERT(file_manager_.use_count()                > 0);
        BLOWBOX_ASSERT(texture_manager_.use_count()             > 0);
        BLOWBOX_ASSERT(material_manager_.use_count()            > 0);

        finalized_ = true;
    }

    //------------------------------------------------------------------------------------------------------
	BlowboxCore* Get::BlowboxCore()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->blowbox_core_;
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<GLFWManager> Get::GLFWManager()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->glfw_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr< Window> Get::MainWindow()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->main_window_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<ForwardRenderer> Get::ForwardRenderer()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->forward_renderer_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<DeferredRenderer> Get::DeferredRenderer()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->deferred_renderer_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<CommandContextManager> Get::CommandContextManager()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->command_context_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<CommandManager> Get::CommandManager()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->command_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<Device> Get::Device()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->device_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<DescriptorHeap> Get::RtvHeap()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->rtv_descriptor_heap_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<DescriptorHeap> Get::DsvHeap()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->dsv_descriptor_heap_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<DescriptorHeap> Get::CbvSrvUavHeap()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->cbv_srv_uav_descriptor_heap_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<SwapChain> Get::SwapChain()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->swap_chain_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<ImGuiManager> Get::ImGuiManager()
    {
        BLOWBOX_ASSERT(Get::instance_->finalized_ == true);
        return Get::instance_->imgui_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
	SharedPtr<SceneManager> Get::SceneManager()
    {
        return Get::instance_->scene_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<DebugMenu> Get::DebugMenu()
    {
        return Get::instance_->debug_menu_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<Console> Get::Console()
    {
        return Get::instance_->console_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<Time> Get::Time()
    {
        return Get::instance_->time_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<PerformanceProfiler> Get::PerformanceProfiler()
    {
        return Get::instance_->performance_profiler_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<MemoryProfiler> Get::MemoryProfiler()
    {
        return Get::instance_->memory_profiler_.lock();
    }
    
    //------------------------------------------------------------------------------------------------------
    SharedPtr<FrameStats> Get::FrameStats()
    {
        return Get::instance_->frame_stats_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<MemoryStats> Get::MemoryStats()
    {
        return Get::instance_->memory_stats_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<ImageManager> Get::ImageManager()
    {
        return Get::instance_->image_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<FileManager> Get::FileManager()
    {
        return Get::instance_->file_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<TextureManager> Get::TextureManager()
    {
        return Get::instance_->texture_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<MaterialManager> Get::MaterialManager()
    {
        return Get::instance_->material_manager_.lock();
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(blowbox::BlowboxCore* blowbox_core)
    {
		blowbox_core_ = blowbox_core;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::GLFWManager> glfw_manager)
    {
        glfw_manager_ = glfw_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::Window> main_window)
    {
        main_window_ = main_window;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::ForwardRenderer> renderer)
    {
        forward_renderer_ = renderer;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::DeferredRenderer> deferred_renderer)
    {
        deferred_renderer_ = deferred_renderer;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::CommandContextManager> command_context_manager)
    {
        command_context_manager_ = command_context_manager;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::CommandManager> command_manager)
    {
        command_manager_ = command_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::Device> device)
    {
        device_ = device;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetRtvHeap(SharedPtr<blowbox::DescriptorHeap> heap)
    {
        rtv_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetDsvHeap(SharedPtr<blowbox::DescriptorHeap> heap)
    {
        dsv_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::SetCbvSrvUavHeap(SharedPtr<blowbox::DescriptorHeap> heap)
    {
        cbv_srv_uav_descriptor_heap_ = heap;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::SwapChain> swap_chain)
    {
        swap_chain_ = swap_chain;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::ImGuiManager> imgui_manager)
    {
        imgui_manager_ = imgui_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::SceneManager> scene_manager)
    {
        scene_manager_ = scene_manager;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::DebugMenu> debug_menu)
    {
        debug_menu_ = debug_menu;
    }
    
    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::Console> console)
    {
        console_ = console;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::Time> time)
    {
        time_ = time;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::PerformanceProfiler> instance)
    {
        performance_profiler_ = instance;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::MemoryProfiler> instance)
    {
        memory_profiler_ = instance;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::FrameStats> instance)
    {
        frame_stats_ = instance;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::MemoryStats> instance)
    {
        memory_stats_ = instance;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::ImageManager> instance)
    {
        image_manager_ = instance;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::FileManager> instance)
    {
        file_manager_ = instance;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::TextureManager> instance)
    {
        texture_manager_ = instance;
    }

    //------------------------------------------------------------------------------------------------------
    void Get::Set(SharedPtr<blowbox::MaterialManager> instance)
    {
        material_manager_ = instance;
    }
}