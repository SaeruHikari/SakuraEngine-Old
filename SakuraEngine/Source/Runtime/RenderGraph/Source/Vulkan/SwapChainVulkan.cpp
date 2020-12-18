#include "RenderGraphVulkan/RenderGraphVulkan.h"
#include "vulkan/vulkan_win32.h"
#include "System/Log.h"

using namespace sakura::graphics;
using namespace sakura::graphics::vk;

sakura::graphics::vk::SwapChain::SwapChain(
	const SwapChainHandle handle, const vk::RenderDevice& dev, const SwapChainDesc& desc)
	:_window(desc.window), _handle(handle), _buffer_count(desc.buffer_count), device(dev)
{
	_extent = Window::extent(_window);

	surface = dev.create_surface(_window);
}

sakura::graphics::vk::SwapChain::~SwapChain()
{
	vkDestroySurfaceKHR(device.instance, surface, nullptr);
}

sakura::uint8 sakura::graphics::vk::SwapChain::buffer_count() const
{
	return static_cast<sakura::uint8>(_buffer_count);
}

sakura::graphics::extent2d sakura::graphics::vk::SwapChain::extent() const
{
	return _extent;
}

sakura::Window sakura::graphics::vk::SwapChain::window() const
{
	return _window;
}

sakura::graphics::RenderGraphHandle sakura::graphics::vk::SwapChain::handle() const
{
	return _handle;
}

sakura::graphics::ETextureFormat sakura::graphics::vk::SwapChain::render_format() const
{
	return ETextureFormat::R8G8B8A8_UNORM;
}

bool sakura::graphics::vk::SwapChain::present()
{
	return false;
}
