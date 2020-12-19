#pragma once
#include "RenderGraph\RenderGraphCore.h"

namespace sakura::graphics::vk
{
	class RenderDevice;
	const int MAX_FRAMES_IN_FLIGHT = 2;
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

		sakura::vector<VkSemaphore> imageAvailableSemaphores;
		sakura::vector<VkSemaphore> renderFinishedSemaphores;
		sakura::vector<VkFence> inFlightFences;
		sakura::vector<VkFence> imagesInFlight;
	protected:
		extent2d extent_;
		const vk::RenderDevice& device_;
		uint8_t current_back_index_ = 0;
	};
}