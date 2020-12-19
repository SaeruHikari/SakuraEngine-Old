#pragma once
#include "RenderGraph\RenderGraphCore.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI SwapChain : public ISwapChain
	{
	public:
		SwapChain(const SwapChainHandle handle,
			const vk::RenderDevice& dev, const SwapChainDesc& desc);
		~SwapChain();
		uint8 buffer_count() const override;
		extent2d extent() const override;
		Window window() const override;
		RenderGraphHandle handle() const override;
		ETextureFormat render_format() const override;
		bool present() override;
		
		const SwapChainHandle _handle;
		const Window _window;

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		VkSwapchainKHR swap_chain = VK_NULL_HANDLE;
		sakura::vector<VkImage> swapchian_images;

		VkFormat format_;

	protected:
		extent2d _extent;

		const vk::RenderDevice& device;
	};
}