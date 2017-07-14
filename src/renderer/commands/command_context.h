#pragma once

#include "util/string.h"

#include "renderer/buffers/gpu_resource.h"
#include "renderer/buffers/gpu_buffer.h"
#include "renderer/buffers/structured_buffer.h"
#include "renderer/buffers/byte_address_buffer.h"
#include "renderer/buffers/depth_buffer.h"
#include "renderer/buffers/color_buffer.h"
#include "renderer/buffers/upload_buffer.h"

#define COMMAND_CONTEXT_BARRIER_BUFFER_SIZE 16

namespace blowbox
{
    /**
    * @brief Represents a 32 bit parameter, which can either be a float, uint or int.
    */
	struct Param32Bit
	{
		Param32Bit(FLOAT f_) : f(f_) {}
		Param32Bit(UINT u_) : u(u_) {}
		Param32Bit(INT i_) : i(i_) {}

		void operator=(FLOAT f_) { f = f_; }
		void operator=(UINT u_) { u = u_; }
		void operator=(INT i_) { i = i_; }

		union
		{
			FLOAT f;
			UINT u;
			INT i;
		};
	};

	class GraphicsContext;
	class ComputeContext;

    /**
    * @brief Utility class - if you inherit from this, the class becomes non-copyable.
    */
	struct NonCopyable
	{
		NonCopyable() = default;                                //!< Constructor.
		NonCopyable(const NonCopyable&) = delete;               //!< Delete the copy constructor.
		NonCopyable& operator=(const NonCopyable&) = delete;    //!< Delete the copy assignment operator.
	};

    /**
    * This is a wrapper that makes it very simple and straightforward to interact with command lists.
    * Start a new CommandContext by calling CommandContext::Begin(name) and complete it by calling 
    * ComandContext::Finish() or CommandContext::Flush().
    *
    * @brief Wrapper around D3D12 command lists.
    */
	class CommandContext : NonCopyable
	{
		friend class CommandContextManager;
	private:
        /**
        * @brief Constructs a CommandContext.
        * @param[in] type The type of command list this CommandContext should represent.
        */
		CommandContext(D3D12_COMMAND_LIST_TYPE type);

        /**
        * @brief Resets the CommandContext.
        * @remarks This essentially frees all the resources this CommandContext was using, and requests new ones, so that the CommandContext can be re-used.
        */
		void Reset();

	public:
        /** @brief Destructs the CommandContext. */
		~CommandContext();

        /**
        * Use this function to start a new CommandContext. It will collect all necessary resources for you to start using it.
        *
        * @brief Begins a new CommandContext.
        * @param[in] name The name of this new CommandContext.
        */
		static CommandContext& Begin(const WString& name = L"");

        /** @brief Converts this CommandContext into a GraphicsContext. */
		GraphicsContext& GetGraphicsContext();

        /** @brief Converts this CommandContext into a ComputeContext. */
		ComputeContext& GetComputeContext();

        /** @brief Initializes the CommandContext, by creating all the resources that it needs. */
		void Initialize();

        /**
        * @brief Initialize a Texture resource with supplied data.
        * @param[in] dest_resource The resource which has to be initialized with the data.
        * @param[in] num_subresources The number of subresources that have to be set in the dest_resource.
        * @param[in] subresource_data The actual data that you want the dest_resource to be initialized with.
        */
		static void InitializeTexture(GpuResource& dest_resource, UINT num_subresources, D3D12_SUBRESOURCE_DATA subresource_data[]);
		
        /**
        * @brief Initialize a Buffer resource with supplied data.
        * @param[in] dest_resource The resource which has to be initialized with the data.
        * @param[in] data The data that should be put into the dest_resource.
        * @param[in] num_bytes The number of bytes in data.
        * @param[in] use_offset Whether or not to use a certain offset when copying the data into the dest_resource.
        * @param[in] offset The offset that should be used when copying the data into the dest_resource.
        */
        static void InitializeBuffer(GpuResource& dest_resource, const void* data, UINT num_bytes, bool use_offset = false, UINT offset = 0);

        /**
        * @brief Copies a resource into another resource.
        * @param[in] dest_resource The resource into which the source_resource should be copied.
        * @param[in] source_resource The resource that contains the data to be copied over into the dest_resource.
        */
		void CopyBuffer(GpuResource& dest_resource, UploadBuffer& source_resource);

        /**
        * @brief Copies a resource into another resource.
        * @param[in] dest_resource The resource into which the source_resource should be copied.
        * @param[in] source_resource The resource that contains the data to be copied over into the dest_resource.
        */
		void CopyBuffer(GpuResource& dest_resource, GpuResource& source_resource);

        /**
        * @brief Copies a region of a resource into another resource.
        * @param[in] dest_resource The resource into which the source_resource should be copied.
        * @param[in] dest_offset The offset in the dest_resource into which will be copied.
        * @param[in] source_resource The resource that contains the data to be copied over into the dest_resource.
        * @param[in] source_offset The offset in the source_resource from where will be copied.
        * @param[in] num_bytes The number of bytes from the offset on that should be copied.
        */
		void CopyBufferRegion(GpuResource& dest_resource, UINT dest_offset, GpuResource& source_resource, UINT source_offset, UINT num_bytes);

        /**
        * @brief Copies a subresource from one resource into another
        * @param[in] dest_resource The resource into which the source_resource should be copied.
        * @param[in] dest_subresource_index The index into which the source subresource should be copied.
        * @param[in] source_resource The resource that contains the data to be copied over into the dest_resource.
        * @param[in] source_subresource_index The index from where the source subresource should be copied.
        */
		void CopySubresource(GpuResource& dest_resource, UINT dest_subresource_index, GpuResource& source_resource, UINT source_subresource_index);

        /**
        * @brief Copies a Counter buffer from a StructuredBuffer into another resource.
        * @param[in] dest_resource The resource into which the counter buffer should be copied.
        * @param[in] dest_offset The offset in the dest_resource into which will be copied.
        * @param[in] source_resource The StructuredBuffer from which the Counter buffer should be copied.
        */
		void CopyCounter(GpuResource& dest_resource, UINT dest_offset, StructuredBuffer& source_resource);

        /**
        * @brief This flushes the CommandContext. That means that it will submit everything to the correct CommandQueue, without releasing its resources.
        * @param[in] wait_for_completion Whether the function should be blocking until all the work has been completed by the GPU.
        */
		uint64_t Flush(bool wait_for_completion = false);

        /**
        * @brief This finishes the CommandContext. That means that it will submit everything to the correct CommandQueue, while also releasing its resources.
        * @param[in] wait_for_completion Whether the function should be blocking until all the work has been completed by the GPU.
        */
		uint64_t Finish(bool wait_for_completion = false);

        /**
        * @brief Transitions a GpuResource into another state.
        * @param[in] resource The resource that should be transitioned.
        * @param[in] new_state The new state that the resource should be in.
        * @param[in] flush_immediate Whether the transition should be pushed through immediately to the command list.
        */
		void TransitionResource(GpuResource& resource, const D3D12_RESOURCE_STATES& new_state, bool flush_immediate = false);

        /**
        * @brief Begins to transition a GpuResource into another state.
        * @param[in] resource The resource that should be transitioned.
        * @param[in] new_state The new state that the resource should be in.
        * @param[in] flush_immediate Whether the transition should be pushed through immediately to the command list.
        */
		void BeginResourceTransition(GpuResource& resource, const D3D12_RESOURCE_STATES& new_state, bool flush_immediate = false);

        /**
        * @brief Inserts a UAV barrier.
        * @param[in] resource The resource that gets transitioned to a UAV state.
        * @param[in] flush_immediate Whether the transition should be pushed through immediately to the command list.
        */
		void InsertUAVBarrier(GpuResource& resource, bool flush_immediate = false);

        /**
        * @brief Inserts an alias barrier.
        * @param[in] before fuck if i know
        * @param[in] after fuck if i know
        * @param[in] flush_immediate Whether the transition should be pushed through immediately to the command list.
        */
		void InsertAliasBuffer(GpuResource& before, GpuResource& after, bool flush_immediate = false);

        /** @brief Flushes all queued up resource barriers by submitting them to the command list. */
		void FlushResourceBarriers();

        /**
        * @brief Inserts a timestamp. Fuck if I know what that means.
        * @param[in] query_heap no clue
        * @param[in] query_idx no clue
        */
		void InsertTimeStamp(ID3D12QueryHeap* query_heap, UINT query_idx);

        /**
        * @brief Resolves the timestamps. No clue man.
        * @param[in] read_back_heap No clue.
        * @param[in] query_heap No clue.
        * @param[in] num_queries No clue.
        */
		void ResolveTimeStamps(ID3D12Resource* read_back_heap, ID3D12QueryHeap* query_heap, UINT num_queries);

        /**
        * @brief Set a descriptor heap that is bound to the pipeline.
        * @param[in] type The type that you want bound to the pipeline.
        * @param[in] heap The descriptor heap that should be bound to the pipeline.
        */
		void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12DescriptorHeap* heap);

        /**
        * @brief Sets the descriptor heaps that should be bound to the pipeline.
        * @param[in] heap_count The number of heaps that need to be bound.
        * @param[in] heap_types The types of heaps you want bound to the pipeline.
        * @param[in] heaps The actual descriptor heaps that you want to bind to the pipeline.
        */
		void SetDescriptorHeaps(UINT heap_count, D3D12_DESCRIPTOR_HEAP_TYPE heap_types[], ID3D12DescriptorHeap* heaps[]);

        /**
        * @brief Sets a predication.
        * @param[in] buffer Yo i dunno man
        * @param[in] buffer_offset Still dont know
        * @param[in] op Definitely don't know now
        */
		void SetPredication(ID3D12Resource* buffer, UINT64 buffer_offset, D3D12_PREDICATION_OP op);

        /** @return The underlying command list. */
		ID3D12GraphicsCommandList* Get();

	protected:
        /** @brief Actually binds the descriptor heaps to the pipeline. */
		void BindDescriptorHeaps();

	protected:
		WString name_; //!< The name of the CommandContext.

        /**
        * @brief Sets the name of the CommandContext.
        * @param[in] name The name of the CommandContext.
        */
        void SetName(const WString& name) { list_->SetName(name.c_str()); name_ = name; }

	protected:
		D3D12_COMMAND_LIST_TYPE type_;                                                          //!< The type of CommandContext this is.
		ID3D12GraphicsCommandList* list_;                                                       //!< The actual underlying command list.
		ID3D12CommandAllocator* allocator_;                                                     //!< The allocator used to allocate the commands.

		UINT num_flushable_barriers_;                                                           //!< The number of resource barriers that are queued up.
		D3D12_RESOURCE_BARRIER resource_barrier_buffer_[COMMAND_CONTEXT_BARRIER_BUFFER_SIZE];   //!< The actual queued up resource barriers.

		ID3D12DescriptorHeap* descriptor_heaps_[4];                                             //!< All DescriptorHeaps that are currently bound.

		ID3D12RootSignature* graphics_root_signature_;                                          //!< The graphics root signature that is currently bound.
		ID3D12PipelineState* graphics_pipeline_state_;                                          //!< The GraphicsPSO that is currently bound.
		ID3D12RootSignature* compute_root_signature_;                                           //!< The compute root signature that is currently bound.
		ID3D12PipelineState* compute_pipeline_state_;                                           //!< The ComputePSO that is currently bound.
	};
}