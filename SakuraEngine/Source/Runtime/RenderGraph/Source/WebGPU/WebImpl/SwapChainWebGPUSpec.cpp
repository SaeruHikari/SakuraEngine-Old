// WebGPU Implementation of WebGPU SwapChain.
#include <RenderGraphWebGPU/RenderGraphWebGPU.h>
#include <System/Log.h>

using namespace sakura::graphics;
using namespace sakura::graphics::webgpu;

void SwapChain::initPlatformSpecific(const SwapChainHandle handle, const webgpu::RenderDevice& dev,
	const SwapChainDescriptor& desc)
{
	auto& window = desc.window;
	WGPUSurfaceDescriptorFromCanvasHTMLSelector canvDesc = {};
	canvDesc.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
	canvDesc.selector = "canvas";
	
	WGPUSurfaceDescriptor surfDesc = {};
	surfDesc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&canvDesc);
	
	WGPUSurface surface = wgpuInstanceCreateSurface(nullptr, &surfDesc);
	
	_extent = sakura::Window::extent(window);
	_format = WGPUTextureFormat_BGRA8Unorm;
	WGPUSwapChainDescriptor swapDesc = {};
	swapDesc.usage  = WGPUTextureUsage_OutputAttachment;
	swapDesc.format = WGPUTextureFormat_BGRA8Unorm;
	swapDesc.width  = _extent.width;
	swapDesc.height = _extent.height;
	// Most Browsers do not support mailbox mode.
	swapDesc.presentMode = WGPUPresentMode_Fifo;
	//swapDesc.presentMode = translate(desc.present_mode);

	swapchain = wgpuDeviceCreateSwapChain(dev.device, surface, &swapDesc);
}
