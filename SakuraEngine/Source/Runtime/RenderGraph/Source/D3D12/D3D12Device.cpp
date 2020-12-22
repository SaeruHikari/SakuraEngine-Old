#include "RenderGraphD3D12/RenderGraphD3D12.h"
#include "System/Log.h"

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <tchar.h>
#include <wrl.h>
using namespace Microsoft;
using namespace Microsoft::WRL;

#include <dxgi1_6.h>

#include <DirectXMath.h>
using namespace DirectX;
//for d3d12

#include <d3d12shader.h>
#include <d3dcompiler.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#include "RenderGraphD3D12/D3D12Device.h"

using namespace sakura::graphics::d3d12;
using namespace sakura::graphics;

RenderDevice::RenderDevice(const sakura::graphics::DeviceConfiguration& config)
	:name(config.name)
{
	static UINT nDXGIFactoryFlags = 0U;
	static bool bInitialized = false;
	static IDXGIFactory5* factory = nullptr;
	static IDXGIAdapter1* adapter = nullptr;
	if(!bInitialized)
	{
		try_exec(CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&factory)));
		try_exec(factory->MakeWindowAssociation(
			reinterpret_cast<HWND>(config.window_handle.handle()), DXGI_MWA_NO_ALT_ENTER));
		for (UINT a_index = 1; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(a_index, &adapter); ++a_index)
		{
			DXGI_ADAPTER_DESC1 desc = {};
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;
			if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr)))
				break;
		}
	}
	bInitialized = true;
	ID3D12Device4* device_to_create = nullptr;
	try_exec(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&d3d12_device)));

	create_graphics_queue();
	async_available = true;
}

RenderShaderHandle RenderDevice::create_shader(const RenderShaderHandle handle, const ShaderDesc& config)
{

	return handle;
}

void RenderDevice::create_graphics_queue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ID3D12CommandQueue* queue_to_create = nullptr;
	try_exec(d3d12_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue_to_create)));
	//graphics_queue
	
}

RenderDevice::RenderDevice()
{
	
}

bool RenderDevice::available_async_compute() const
{
	return async_available;
}

RenderBufferHandle RenderDevice::create_buffer(const RenderBufferHandle handle, const BufferDesc& config)
{
	return handle;
}

SwapChainHandle RenderDevice::create_swap_chain(
	const SwapChainHandle handle, const SwapChainDesc& window)
{
	sakura::warn("D3D12 SwapChain Not Implemented Yet!");
	return handle;
}

RenderPipelineHandle RenderDevice::create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc)
{
	sakura::warn("D3D12 RenderPipeline Not Implemented Yet!");
	return handle;
}

sakura::string_view RenderDevice::get_name() const
{
	return name;
}

EBackend RenderDevice::backend() const
{
	return EBackend::D3D12;
}

sakura::graphics::d3d12::RenderDevice::~RenderDevice()
{
	
}

bool RenderDevice::valid(const RenderShaderHandle shader) const
{
	return false;
}

bool RenderDevice::execute(const RenderGraph& graph_to_execute)
{
	return true;
}

void RenderDevice::destroy_resource(const RenderShaderHandle to_destroy)
{

}

void RenderDevice::terminate()
{

}