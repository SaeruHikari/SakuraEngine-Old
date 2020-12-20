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
		

		VkImageView back_buffer() const;
		const SwapChainHandle handle_;
		const Window window_;
		VkSurfaceKHR surface_ = VK_NULL_HANDLE;
		VkSwapchainKHR swap_chain_ = VK_NULL_HANDLE;
		sakura::vector<VkImage> images_;
		sakura::vector<VkImageView> image_views_;
		VkFormat format_;

		VkSemaphore presentCompleteSemaphore;
		sakura::vector<VkSemaphore> render_to_screen_semaphores_;

		uint32_t imageIndex = 0;
	protected:
		extent2d extent_;
		const vk::RenderDevice& device_;
	};
}