#include "RenderGraphWebGPU/RenderGraphWebGPU.h"
#include "system/Log.h"

using namespace sakura::graphics::webgpu;

SwapChain::SwapChain(const SwapChainHandle handle,
	const webgpu::RenderDevice& dev, const SwapChainDesc& desc)
	:_handle(handle), _window(desc.window), _buffer_count(desc.buffer_count)
{
	initPlatformSpecific(handle, dev, desc);
	
	if (!swapchain)
	{
		sakura::error("[SwapChain WebGPU]Error: Create Failed!");
	}
	else
	{
		sakura::debug("[SwapChain WebGPU]Info: Created Succeed! Size: X-{} Y-{}", _extent.width, _extent.height);
	}
}

SwapChain::~SwapChain()
{
	wgpuSwapChainRelease(swapchain);
}

sakura::uint8 SwapChain::buffer_count() const
{
	return _buffer_count;
}

sakura::extent2d SwapChain::extent() const
{
	return _extent;
}

sakura::Window SwapChain::window() const
{
	return _window;
}

sakura::graphics::RenderGraphHandle SwapChain::handle() const
{
	return _handle;
}

sakura::graphics::ETextureFormat SwapChain::render_format() const
{
	return translate(_format);
}

bool SwapChain::present()
{
	 wgpuSwapChainPresent(swapchain);
	 return swapchain;
}

