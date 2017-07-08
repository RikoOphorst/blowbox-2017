#include "renderer.h"

#include "renderer/commands/command_context_manager.h"
#include "renderer/commands/command_manager.h"
#include "renderer/commands/command_context.h"
#include "renderer/commands/graphics_context.h"
#include "renderer/commands/command_queue.h"
#include "renderer/device.h"
#include "renderer/descriptor_heap.h"
#include "renderer/root_signature.h"
#include "renderer/pipeline_state.h"
#include "renderer/constants_helper.h"
#include "renderer/material.h"
#include "renderer/shader.h"
#include "renderer/vertex.h"
#include "renderer/graphics.h"
#include "win32/window.h"
#include "core/get.h"

#define SWAP_CHAIN_BUFFER_COUNT 2

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	Renderer::Renderer() :
		num_object_cbs_(0),
		num_material_cbs_(0)
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	Renderer::~Renderer()
	{
		FlushCommandQueue();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::Startup()
	{
		CreateDevice();
		Graphics::Initialize();
		CreateCommandObjects();
		CreateDescriptorHeaps();
		CreateSwapChain();
		CreateBuffers();
		CreatePSOs();
		FlushCommandQueue();

		buffer_manager_.Create(swap_chain_.GetBufferWidth(), swap_chain_.GetBufferHeight());
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateDevice()
	{
#ifdef _DEBUG
		ID3D12Debug* debug_controller;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
		{
			debug_controller->EnableDebugLayer();
		}
		debug_controller->Release();
#endif

        eastl::vector<Adapter> adapters = Device::FindAvailableAdapters(static_cast<FindAdapterFlag>(FindAdapterFlag_NO_INTEL | FindAdapterFlag_NO_SOFTWARE));
        BLOWBOX_ASSERT(adapters.size() > 0); // there should be at least 1 adapter

		device_.Create(adapters[0]);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateCommandObjects()
	{
		Get::CommandManager()->Startup(device_);
		Get::CommandContextManager()->Startup();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateSwapChain()
	{
		swap_chain_.Create(
			Get::MainWindow()->GetWindowHandle(),
            Get::MainWindow()->GetWindowResolution().width,
            Get::MainWindow()->GetWindowResolution().height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			SWAP_CHAIN_BUFFER_COUNT,
			DXGI_SWAP_EFFECT_FLIP_DISCARD,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateDescriptorHeaps()
	{
		rtv_heap_.Create(			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,			D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 4096U);
		dsv_heap_.Create(			D3D12_DESCRIPTOR_HEAP_TYPE_DSV,			D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 4096U);
		cbv_srv_uav_heap_.Create(	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 4096U * 8);
		sampler_heap_.Create(		D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1024U);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreateBuffers()
	{
		depth_buffer_.Create(L"SceneDepthBuffer", swap_chain_.GetBufferWidth(), swap_chain_.GetBufferHeight(), DXGI_FORMAT_D32_FLOAT);
		object_constants_.Create(L"ObjectConstantsBuffer", 1024U, (sizeof(ObjectConstants) + 255) & ~255);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CreatePSOs()
	{
        shader_vs_ = new Shader();
        shader_vs_->Create(new TextFile("vertex_shader.hlsl"), ShaderType_VERTEX);
        shader_ps_ = new Shader();
        shader_ps_->Create(new TextFile("pixel_shader.hlsl"), ShaderType_PIXEL);

		GraphicsPSO& render_lit = GraphicsPSO::Get("render_lit");
		render_lit.SetRootSignature(Graphics::root_signature_default);
		render_lit.SetVertexShader(shader_vs_->GetShaderByteCode());
		render_lit.SetPixelShader(shader_ps_->GetShaderByteCode());
		render_lit.SetBlendState(Graphics::blend_state_traditional);
		render_lit.SetDepthStencilState(Graphics::depth_state_default);
		render_lit.SetRasterizerState(Graphics::rasterizer_default_cw);
		render_lit.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		render_lit.SetInputLayout(_countof(input_element_vertex_desc), input_element_vertex_desc);
		render_lit.SetSampleMask(0xFFFFFFFF);
		render_lit.SetRenderTargetFormat(swap_chain_.GetBackBuffer().GetFormat(), depth_buffer_.GetFormat());
		render_lit.Finalize();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::Draw()
	{
		GraphicsContext& context = GraphicsContext::Begin(L"SceneRender");
		
		ID3D12DescriptorHeap* heaps[2] = { cbv_srv_uav_heap_.Get(), sampler_heap_.Get() };
		D3D12_DESCRIPTOR_HEAP_TYPE heap_types[2] = { D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER };
		
		context.SetDescriptorHeaps(2, heap_types, heaps);
		context.TransitionResource(swap_chain_.GetBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET);
		
		context.SetRenderTarget(swap_chain_.GetBackBuffer().GetRTV(), depth_buffer_.GetDSV());
		context.ClearColor(swap_chain_.GetBackBuffer());
		
		context.TransitionResource(depth_buffer_, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
		context.ClearDepth(depth_buffer_);

		context.SetViewportAndScissor(0, 0, swap_chain_.GetBufferWidth(), swap_chain_.GetBufferHeight());
		
		context.FlushResourceBarriers();
		
		context.TransitionResource(swap_chain_.GetBackBuffer(), D3D12_RESOURCE_STATE_PRESENT);
		
		CheckDeviceRemoved();
		
		context.Finish(true);

		swap_chain_.Present(false);
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::FlushCommandQueue()
	{
		Get::CommandManager()->WaitForIdleGPU();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::CheckDeviceRemoved()
	{
		HRESULT hr = device_.Get()->GetDeviceRemovedReason();
		if (hr != S_OK)
		{
			//_com_error error(hr);
			//std::cout << error.ErrorMessage() << std::endl;
			assert(false);
		}
	}
}