#include "memory_profiler.h"

#include "core/get.h"
#include "win32/window.h"

#include "util/sort.h"
#include "util/algorithm.h"

#include "renderer/buffers/gpu_resource.h"
#include "renderer/descriptor_heap.h"

#include <locale>
#include <codecvt>

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    MemoryProfiler::MemoryProfiler() :
        show_window_(false),
        condense_(true),
        is_condensed_(false),
        has_shutdown_(false),
        sort_type_(ResourceSortType_NAME)
    {
        Condense();
    }

    //------------------------------------------------------------------------------------------------------
    MemoryProfiler::~MemoryProfiler()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::RenderMenu()
    {
        if (ImGui::BeginMenu("Memory Profiler"))
        {
            if (!show_window_)
            {
                if (ImGui::MenuItem("Show Memory Profiler", "CTRL+5", false, !show_window_))
                {
                    show_window_ = true;
                }
            }
            else
            {
                if (ImGui::MenuItem("Hide Memory Profiler", "CTRL+5", false, show_window_))
                {
                    show_window_ = false;
                }
            }

            ImGui::EndMenu();
        }

        KeyboardState& keyboard = Get::MainWindow()->GetKeyboardState();

        if (keyboard.GetKeyDown(KeyCode_LEFT_CONTROL) && keyboard.GetKeyPressed(KeyCode_5))
        {
            show_window_ = !show_window_;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::RenderWindow()
    {
        if (show_window_)
        {
            ImGui::SetNextWindowSize(ImVec2(850, 480), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
            if (ImGui::Begin("Memory Profiler", &show_window_, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
            {
                std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

                filter_.Draw("Filter by name");
                ImGui::Checkbox("Condense by name", &condense_);

                if (condense_)
                {
                    Condense();
                }
                else
                {
                    Uncondense();
                }

                sorted_gpu_resources_.clear();
                if (is_condensed_)
                {
                    for (auto it = gpu_resources_map_.begin(); it != gpu_resources_map_.end(); it++)
                    {
                        sorted_gpu_resources_.push_back(it->second);
                    }
                }
                else
                {
                    for (int i = 0; i < unsorted_gpu_resources_.size(); i++)
                    {
                        D3D12_RESOURCE_DESC desc = unsorted_gpu_resources_[i]->resource_->GetDesc();

                        ResourceData data;
                        data.name = unsorted_gpu_resources_[i]->name_;
                        data.format = desc.Format;
                        data.total_size = desc.Width * desc.Height;
                        data.total_count = 1;
                        data.resource = unsorted_gpu_resources_[i];
                        data.dimension = desc.Dimension;

                        sorted_gpu_resources_.push_back(data);
                    }
                }

                switch (sort_type_)
                {
                default:
                case ResourceSortType_NAME:
                    eastl::stable_sort(sorted_gpu_resources_.begin(), sorted_gpu_resources_.end(), CompareGpuResourceByName());
                    break;
                case ResourceSortType_NAME_REVERTED:
                    eastl::stable_sort(sorted_gpu_resources_.begin(), sorted_gpu_resources_.end(), CompareGpuResourceByNameReverted());
                    break;
                case ResourceSortType_SIZE:
                    eastl::stable_sort(sorted_gpu_resources_.begin(), sorted_gpu_resources_.end(), CompareGpuResourceBySize());
                    break;
                case ResourceSortType_SIZE_REVERTED:
                    eastl::stable_sort(sorted_gpu_resources_.begin(), sorted_gpu_resources_.end(), CompareGpuResourceBySizeReverted());
                    break;
                case ResourceSortType_DIMENSION:
                    eastl::stable_sort(sorted_gpu_resources_.begin(), sorted_gpu_resources_.end(), CompareGpuResourceByDimension());
                    break;
                case ResourceSortType_DIMENSION_REVERTED:
                    eastl::stable_sort(sorted_gpu_resources_.begin(), sorted_gpu_resources_.end(), CompareGpuResourceByDimensionReverted());
                    break;
                case ResourceSortType_FORMAT:
                    eastl::stable_sort(sorted_gpu_resources_.begin(), sorted_gpu_resources_.end(), CompareGpuResourceByFormat());
                    break;
                case ResourceSortType_FORMAT_REVERTED:
                    eastl::stable_sort(sorted_gpu_resources_.begin(), sorted_gpu_resources_.end(), CompareGpuResourceByFormatReverted());
                    break;
                }

                ImGui::Separator();

                ImGui::BeginChild("Resource Overview", ImVec2(820, 200), true);
                if (is_condensed_)
                {
                    ImGui::Columns(4);
                    if (ImGui::Button("Name (Count)"))
                    {
                        if (sort_type_ == ResourceSortType_NAME)
                        {
                            sort_type_ = ResourceSortType_NAME_REVERTED;
                        }
                        else
                        {
                            sort_type_ = ResourceSortType_NAME;
                        }
                    }
                    ImGui::NextColumn();
                    if (ImGui::Button("Total size"))
                    {
                        if (sort_type_ == ResourceSortType_SIZE)
                        {
                            sort_type_ = ResourceSortType_SIZE_REVERTED;
                        }
                        else
                        {
                            sort_type_ = ResourceSortType_SIZE;
                        }
                    }
                    ImGui::NextColumn();
                    if (ImGui::Button("Dimension"))
                    {
                        if (sort_type_ == ResourceSortType_DIMENSION)
                        {
                            sort_type_ = ResourceSortType_DIMENSION_REVERTED;
                        }
                        else
                        {
                            sort_type_ = ResourceSortType_DIMENSION;
                        }
                    }
                    ImGui::NextColumn();
                    if (ImGui::Button("Format"))
                    {
                        if (sort_type_ == ResourceSortType_FORMAT)
                        {
                            sort_type_ = ResourceSortType_FORMAT_REVERTED;
                        }
                        else
                        {
                            sort_type_ = ResourceSortType_FORMAT;
                        }
                    }
                    ImGui::Separator();
                    ImGui::NextColumn();

                    for (int i = 0; i < sorted_gpu_resources_.size(); i++)
                    {
                        ResourceData& current = sorted_gpu_resources_[i];
                        String name = converter.to_bytes(current.name.c_str()).c_str();

                        if (filter_.PassFilter(name.c_str()))
                        {
                            ImGui::Text("%s (%i)", name.c_str(), current.total_count);
                            ImGui::NextColumn();
                            ImGui::Text("%g KB", (float)current.total_size / 1000.0f);
                            ImGui::NextColumn();
                            ImGui::Text("%s", ConvertDimensionToString(current.dimension).c_str());
                            ImGui::NextColumn();
                            ImGui::Text("%s", ConvertFormatToString(current.format).c_str());
                            ImGui::NextColumn();
                        }
                    }
                    ImGui::Columns(1);
                }
                else
                {
                    ImGui::Columns(4);
                    if (ImGui::Button("Name"))
                    {
                        if (sort_type_ == ResourceSortType_NAME)
                        {
                            sort_type_ = ResourceSortType_NAME_REVERTED;
                        }
                        else
                        {
                            sort_type_ = ResourceSortType_NAME;
                        }
                    }
                    ImGui::NextColumn();
                    if (ImGui::Button("Size"))
                    {
                        if (sort_type_ == ResourceSortType_SIZE)
                        {
                            sort_type_ = ResourceSortType_SIZE_REVERTED;
                        }
                        else
                        {
                            sort_type_ = ResourceSortType_SIZE;
                        }
                    }
                    ImGui::NextColumn();
                    if (ImGui::Button("Dimension"))
                    {
                        if (sort_type_ == ResourceSortType_DIMENSION)
                        {
                            sort_type_ = ResourceSortType_DIMENSION_REVERTED;
                        }
                        else
                        {
                            sort_type_ = ResourceSortType_DIMENSION;
                        }
                    }
                    ImGui::NextColumn();
                    if (ImGui::Button("Format"))
                    {
                        if (sort_type_ == ResourceSortType_FORMAT)
                        {
                            sort_type_ = ResourceSortType_FORMAT_REVERTED;
                        }
                        else
                        {
                            sort_type_ = ResourceSortType_FORMAT;
                        }
                    }
                    ImGui::Separator();
                    ImGui::NextColumn();

                    for (int i = 0; i < sorted_gpu_resources_.size(); i++)
                    {
                        ResourceData& current = sorted_gpu_resources_[i];
                        String name = converter.to_bytes(current.name.c_str()).c_str();

                        if (filter_.PassFilter(name.c_str()))
                        {
                            ImGui::Text("%s", name.c_str());
                            ImGui::NextColumn();
                            ImGui::Text("%g KB", (float)current.total_size / 1000.0f);
                            ImGui::NextColumn();
                            ImGui::Text("%s", ConvertDimensionToString(current.dimension).c_str());
                            ImGui::NextColumn();
                            ImGui::Text("%s", ConvertFormatToString(current.format).c_str());
                            ImGui::NextColumn();
                        }
                    }
                    ImGui::Columns(1);
                }
                ImGui::EndChild();

                ImGui::Separator();

                char buf[512];

                ImGui::Columns(2);
                ImGui::TextUnformatted("RTV descriptor heap maximum descriptor count:");
                ImGui::NextColumn();
                ImGui::Text("%i", Get::RtvHeap()->GetDescriptorHeapMaxAllocations());
                ImGui::NextColumn();
                ImGui::TextUnformatted("RTV descriptor heap descriptors allocated:");
                ImGui::NextColumn();
                ImGui::Text("%i", Get::RtvHeap()->GetDescriptorsAllocated());
                ImGui::Columns(1);
                sprintf(buf, "%.0f%% used of RTV Descriptor Heap", static_cast<float>(Get::RtvHeap()->GetDescriptorsAllocated()) / static_cast<float>(Get::RtvHeap()->GetDescriptorHeapMaxAllocations()) * 100.0f);
                ImGui::ProgressBar(static_cast<float>(Get::RtvHeap()->GetDescriptorsAllocated()) / static_cast<float>(Get::RtvHeap()->GetDescriptorHeapMaxAllocations()), ImVec2(-1.0f, 0.0f), buf);

                ImGui::Separator();

                ImGui::Columns(2);
                ImGui::TextUnformatted("DSV descriptor heap maximum descriptor count:");
                ImGui::NextColumn();
                ImGui::Text("%i", Get::DsvHeap()->GetDescriptorHeapMaxAllocations());
                ImGui::NextColumn();
                ImGui::TextUnformatted("DSV descriptor heap descriptors allocated:");
                ImGui::NextColumn();
                ImGui::Text("%i", Get::DsvHeap()->GetDescriptorsAllocated());
                ImGui::Columns(1); 
                sprintf(buf, "%.0f%% used of DSV Descriptor Heap", static_cast<float>(Get::DsvHeap()->GetDescriptorsAllocated()) / static_cast<float>(Get::DsvHeap()->GetDescriptorHeapMaxAllocations()) * 100.0f);
                ImGui::ProgressBar(static_cast<float>(Get::DsvHeap()->GetDescriptorsAllocated()) / static_cast<float>(Get::DsvHeap()->GetDescriptorHeapMaxAllocations()), ImVec2(-1.0f, 0.0f), buf);

                ImGui::Separator();

                ImGui::Columns(2);
                ImGui::TextUnformatted("CBV / SRV / UAV descriptor heap maximum descriptor count:");
                ImGui::NextColumn();
                ImGui::Text("%i", Get::CbvSrvUavHeap()->GetDescriptorHeapMaxAllocations());
                ImGui::NextColumn();
                ImGui::TextUnformatted("CBV / SRV / UAV descriptor heap descriptors allocated:");
                ImGui::NextColumn();
                ImGui::Text("%i", Get::CbvSrvUavHeap()->GetDescriptorsAllocated());
                ImGui::Columns(1);
                sprintf(buf, "%.0f%% used of CBV / SRV / UAV Descriptor Heap", static_cast<float>(Get::CbvSrvUavHeap()->GetDescriptorsAllocated()) / static_cast<float>(Get::CbvSrvUavHeap()->GetDescriptorHeapMaxAllocations()) * 100.0f);
                ImGui::ProgressBar(static_cast<float>(Get::CbvSrvUavHeap()->GetDescriptorsAllocated()) / static_cast<float>(Get::CbvSrvUavHeap()->GetDescriptorHeapMaxAllocations()), ImVec2(-1.0f, 0.0f), buf);

                ImGui::End();
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::NewFrame()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::Shutdown()
    {
        has_shutdown_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::AddGpuResource(GpuResource* gpu_resource)
    {
        if (!has_shutdown_)
        {
            unsorted_gpu_resources_.push_back(gpu_resource);

            if (is_condensed_)
            {
                Condense(true);
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::RemoveGpuResource(GpuResource* gpu_resource)
    {
        if (!has_shutdown_)
        {
            auto it = eastl::lower_bound(unsorted_gpu_resources_.begin(), unsorted_gpu_resources_.end(), gpu_resource);

            if (it != unsorted_gpu_resources_.end())
            {
                unsorted_gpu_resources_.erase(it);
            }

            if (is_condensed_)
            {
                Condense(true);
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::Condense(bool force)
    {
        if (!is_condensed_ || force)
        {
            gpu_resources_map_.clear();

            for (int i = 0; i < unsorted_gpu_resources_.size(); i++)
            {
                GpuResource* current = unsorted_gpu_resources_[i];

                if (gpu_resources_map_.find(current->name_) == gpu_resources_map_.end())
                {
                    gpu_resources_map_[current->name_].name = current->name_;
                    gpu_resources_map_[current->name_].resource = current;
                    gpu_resources_map_[current->name_].total_count = 0;
                    gpu_resources_map_[current->name_].total_size = 0;
                }

                gpu_resources_map_[current->name_].total_count++;
                gpu_resources_map_[current->name_].total_size += current->resource_->GetDesc().Width * current->resource_->GetDesc().Height;
                gpu_resources_map_[current->name_].format = current->resource_->GetDesc().Format;
                gpu_resources_map_[current->name_].dimension = current->resource_->GetDesc().Dimension;
            }
        }

        is_condensed_ = true;
    }

    //------------------------------------------------------------------------------------------------------
    void MemoryProfiler::Uncondense()
    {
        is_condensed_ = false;
    }

    //------------------------------------------------------------------------------------------------------
    String MemoryProfiler::ConvertDimensionToString(D3D12_RESOURCE_DIMENSION dimension)
    {
        String ret = "UNKNOWN";

        switch (dimension)
        {
            case D3D12_RESOURCE_DIMENSION_BUFFER:           ret = "BUFFER"; break;
            case D3D12_RESOURCE_DIMENSION_TEXTURE1D:        ret = "TEXTURE1D"; break;
            case D3D12_RESOURCE_DIMENSION_TEXTURE2D:        ret = "TEXTURE2D"; break;
            case D3D12_RESOURCE_DIMENSION_TEXTURE3D:        ret = "TEXTURE3D"; break;
        }

        return ret;
    }

    //------------------------------------------------------------------------------------------------------
    String MemoryProfiler::ConvertFormatToString(DXGI_FORMAT format)
    {
        String ret = "UNKNOWN";

        switch (format)
        {
            case DXGI_FORMAT_UNKNOWN: ret = "UNKNOWN"; break;
            case DXGI_FORMAT_R32G32B32A32_TYPELESS: ret = "R32G32B32A32_TYPELESS"; break;
            case DXGI_FORMAT_R32G32B32A32_FLOAT: ret = "R32G32B32A32_FLOAT"; break;
            case DXGI_FORMAT_R32G32B32A32_UINT: ret = "R32G32B32A32_UINT"; break;
            case DXGI_FORMAT_R32G32B32A32_SINT: ret = "R32G32B32A32_SINT"; break;
            case DXGI_FORMAT_R32G32B32_TYPELESS: ret = "R32G32B32_TYPELESS"; break;
            case DXGI_FORMAT_R32G32B32_FLOAT: ret = "R32G32B32_FLOAT"; break;
            case DXGI_FORMAT_R32G32B32_UINT: ret = "R32G32B32_UINT"; break;
            case DXGI_FORMAT_R32G32B32_SINT: ret = "R32G32B32_SINT"; break;
            case DXGI_FORMAT_R16G16B16A16_TYPELESS: ret = "R16G16B16A16_TYPELESS"; break;
            case DXGI_FORMAT_R16G16B16A16_FLOAT: ret = "R16G16B16A16_FLOAT"; break;
            case DXGI_FORMAT_R16G16B16A16_UNORM: ret = "R16G16B16A16_UNORM"; break;
            case DXGI_FORMAT_R16G16B16A16_UINT: ret = "R16G16B16A16_UINT"; break;
            case DXGI_FORMAT_R16G16B16A16_SNORM: ret = "R16G16B16A16_SNORM"; break;
            case DXGI_FORMAT_R16G16B16A16_SINT: ret = "R16G16B16A16_SINT"; break;
            case DXGI_FORMAT_R32G32_TYPELESS: ret = "R32G32_TYPELESS"; break;
            case DXGI_FORMAT_R32G32_FLOAT: ret = "R32G32_FLOAT"; break;
            case DXGI_FORMAT_R32G32_UINT: ret = "R32G32_UINT"; break;
            case DXGI_FORMAT_R32G32_SINT: ret = "R32G32_SINT"; break;
            case DXGI_FORMAT_R32G8X24_TYPELESS: ret = "R32G8X24_TYPELESS"; break;
            case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: ret = "D32_FLOAT_S8X24_UINT"; break;
            case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS: ret = "R32_FLOAT_X8X24_TYPELESS"; break;
            case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT: ret = "X32_TYPELESS_G8X24_UINT"; break;
            case DXGI_FORMAT_R10G10B10A2_TYPELESS: ret = "R10G10B10A2_TYPELESS"; break;
            case DXGI_FORMAT_R10G10B10A2_UNORM: ret = "R10G10B10A2_UNORM"; break;
            case DXGI_FORMAT_R10G10B10A2_UINT: ret = "R10G10B10A2_UINT"; break;
            case DXGI_FORMAT_R11G11B10_FLOAT: ret = "R11G11B10_FLOAT"; break;
            case DXGI_FORMAT_R8G8B8A8_TYPELESS: ret = "R8G8B8A8_TYPELESS"; break;
            case DXGI_FORMAT_R8G8B8A8_UNORM: ret = "R8G8B8A8_UNORM"; break;
            case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: ret = "R8G8B8A8_UNORM_SRGB"; break;
            case DXGI_FORMAT_R8G8B8A8_UINT: ret = "R8G8B8A8_UINT"; break;
            case DXGI_FORMAT_R8G8B8A8_SNORM: ret = "R8G8B8A8_SNORM"; break;
            case DXGI_FORMAT_R8G8B8A8_SINT: ret = "R8G8B8A8_SINT"; break;
            case DXGI_FORMAT_R16G16_TYPELESS: ret = "R16G16_TYPELESS"; break;
            case DXGI_FORMAT_R16G16_FLOAT: ret = "R16G16_FLOAT"; break;
            case DXGI_FORMAT_R16G16_UNORM: ret = "R16G16_UNORM"; break;
            case DXGI_FORMAT_R16G16_UINT: ret = "R16G16_UINT"; break;
            case DXGI_FORMAT_R16G16_SNORM: ret = "R16G16_SNORM"; break;
            case DXGI_FORMAT_R16G16_SINT: ret = "R16G16_SINT"; break;
            case DXGI_FORMAT_R32_TYPELESS: ret = "R32_TYPELESS"; break;
            case DXGI_FORMAT_D32_FLOAT: ret = "D32_FLOAT"; break;
            case DXGI_FORMAT_R32_FLOAT: ret = "R32_FLOAT"; break;
            case DXGI_FORMAT_R32_UINT: ret = "R32_UINT"; break;
            case DXGI_FORMAT_R32_SINT: ret = "R32_SINT"; break;
            case DXGI_FORMAT_R24G8_TYPELESS: ret = "R24G8_TYPELESS"; break;
            case DXGI_FORMAT_D24_UNORM_S8_UINT: ret = "D24_UNORM_S8_UINT"; break;
            case DXGI_FORMAT_R24_UNORM_X8_TYPELESS: ret = "R24_UNORM_X8_TYPELESS"; break;
            case DXGI_FORMAT_X24_TYPELESS_G8_UINT: ret = "X24_TYPELESS_G8_UINT"; break;
            case DXGI_FORMAT_R8G8_TYPELESS: ret = "R8G8_TYPELESS"; break;
            case DXGI_FORMAT_R8G8_UNORM: ret = "R8G8_UNORM"; break;
            case DXGI_FORMAT_R8G8_UINT: ret = "R8G8_UINT"; break;
            case DXGI_FORMAT_R8G8_SNORM: ret = "R8G8_SNORM"; break;
            case DXGI_FORMAT_R8G8_SINT: ret = "R8G8_SINT"; break;
            case DXGI_FORMAT_R16_TYPELESS: ret = "R16_TYPELESS"; break;
            case DXGI_FORMAT_R16_FLOAT: ret = "R16_FLOAT"; break;
            case DXGI_FORMAT_D16_UNORM: ret = "D16_UNORM"; break;
            case DXGI_FORMAT_R16_UNORM: ret = "R16_UNORM"; break;
            case DXGI_FORMAT_R16_UINT: ret = "R16_UINT"; break;
            case DXGI_FORMAT_R16_SNORM: ret = "R16_SNORM"; break;
            case DXGI_FORMAT_R16_SINT: ret = "R16_SINT"; break;
            case DXGI_FORMAT_R8_TYPELESS: ret = "R8_TYPELESS"; break;
            case DXGI_FORMAT_R8_UNORM: ret = "R8_UNORM"; break;
            case DXGI_FORMAT_R8_UINT: ret = "R8_UINT"; break;
            case DXGI_FORMAT_R8_SNORM: ret = "R8_SNORM"; break;
            case DXGI_FORMAT_R8_SINT: ret = "R8_SINT"; break;
            case DXGI_FORMAT_A8_UNORM: ret = "A8_UNORM"; break;
            case DXGI_FORMAT_R1_UNORM: ret = "R1_UNORM"; break;
            case DXGI_FORMAT_R9G9B9E5_SHAREDEXP: ret = "R9G9B9E5_SHAREDEXP"; break;
            case DXGI_FORMAT_R8G8_B8G8_UNORM: ret = "R8G8_B8G8_UNORM"; break;
            case DXGI_FORMAT_G8R8_G8B8_UNORM: ret = "G8R8_G8B8_UNORM"; break;
            case DXGI_FORMAT_BC1_TYPELESS: ret = "BC1_TYPELESS"; break;
            case DXGI_FORMAT_BC1_UNORM: ret = "BC1_UNORM"; break;
            case DXGI_FORMAT_BC1_UNORM_SRGB: ret = "BC1_UNORM_SRGB"; break;
            case DXGI_FORMAT_BC2_TYPELESS: ret = "BC2_TYPELESS"; break;
            case DXGI_FORMAT_BC2_UNORM: ret = "BC2_UNORM"; break;
            case DXGI_FORMAT_BC2_UNORM_SRGB: ret = "BC2_UNORM_SRGB"; break;
            case DXGI_FORMAT_BC3_TYPELESS: ret = "BC3_TYPELESS"; break;
            case DXGI_FORMAT_BC3_UNORM: ret = "BC3_UNORM"; break;
            case DXGI_FORMAT_BC3_UNORM_SRGB: ret = "BC3_UNORM_SRGB"; break;
            case DXGI_FORMAT_BC4_TYPELESS: ret = "BC4_TYPELESS"; break;
            case DXGI_FORMAT_BC4_UNORM: ret = "BC4_UNORM"; break;
            case DXGI_FORMAT_BC4_SNORM: ret = "BC4_SNORM"; break;
            case DXGI_FORMAT_BC5_TYPELESS: ret = "BC5_TYPELESS"; break;
            case DXGI_FORMAT_BC5_UNORM: ret = "BC5_UNORM"; break;
            case DXGI_FORMAT_BC5_SNORM: ret = "BC5_SNORM"; break;
            case DXGI_FORMAT_B5G6R5_UNORM: ret = "B5G6R5_UNORM"; break;
            case DXGI_FORMAT_B5G5R5A1_UNORM: ret = "B5G5R5A1_UNORM"; break;
            case DXGI_FORMAT_B8G8R8A8_UNORM: ret = "B8G8R8A8_UNORM"; break;
            case DXGI_FORMAT_B8G8R8X8_UNORM: ret = "B8G8R8X8_UNORM"; break;
            case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM: ret = "R10G10B10_XR_BIAS_A2_UNORM"; break;
            case DXGI_FORMAT_B8G8R8A8_TYPELESS: ret = "B8G8R8A8_TYPELESS"; break;
            case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: ret = "B8G8R8A8_UNORM_SRGB"; break;
            case DXGI_FORMAT_B8G8R8X8_TYPELESS: ret = "B8G8R8X8_TYPELESS"; break;
            case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: ret = "B8G8R8X8_UNORM_SRGB"; break;
            case DXGI_FORMAT_BC6H_TYPELESS: ret = "BC6H_TYPELESS"; break;
            case DXGI_FORMAT_BC6H_UF16: ret = "BC6H_UF16"; break;
            case DXGI_FORMAT_BC6H_SF16: ret = "BC6H_SF16"; break;
            case DXGI_FORMAT_BC7_TYPELESS: ret = "BC7_TYPELESS"; break;
            case DXGI_FORMAT_BC7_UNORM: ret = "BC7_UNORM"; break;
            case DXGI_FORMAT_BC7_UNORM_SRGB: ret = "BC7_UNORM_SRGB"; break;
            case DXGI_FORMAT_AYUV: ret = "AYUV"; break;
            case DXGI_FORMAT_Y410: ret = "Y410"; break;
            case DXGI_FORMAT_Y416: ret = "Y416"; break;
            case DXGI_FORMAT_NV12: ret = "NV12"; break;
            case DXGI_FORMAT_P010: ret = "P010"; break;
            case DXGI_FORMAT_P016: ret = "P016"; break;
            case DXGI_FORMAT_420_OPAQUE: ret = "420_OPAQUE"; break;
            case DXGI_FORMAT_YUY2: ret = "YUY2"; break;
            case DXGI_FORMAT_Y210: ret = "Y210"; break;
            case DXGI_FORMAT_Y216: ret = "Y216"; break;
            case DXGI_FORMAT_NV11: ret = "NV11"; break;
            case DXGI_FORMAT_AI44: ret = "AI44"; break;
            case DXGI_FORMAT_IA44: ret = "IA44"; break;
            case DXGI_FORMAT_P8: ret = "P8"; break;
            case DXGI_FORMAT_A8P8: ret = "A8P8"; break;
            case DXGI_FORMAT_B4G4R4A4_UNORM: ret = "B4G4R4A4_UNORM"; break;
            case DXGI_FORMAT_P208: ret = "P208"; break;
            case DXGI_FORMAT_V208: ret = "V208"; break;
            case DXGI_FORMAT_V408: ret = "V408"; break;
            case DXGI_FORMAT_FORCE_UINT: ret = "FORCE_UINT"; break;
        }

        return ret;
    }
}