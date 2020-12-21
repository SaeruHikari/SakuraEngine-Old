#pragma once
#include "RenderGraph\RenderGraphCore.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI SwapChain final : public ISwapChain
	{
		friend class RenderDevice;
	public:
		SwapChain(const SwapChainHandle handle,
			const vk::RenderDevice& dev, const SwapChainDesc& desc);
		~SwapChain();
		
		uint8 buffer_count() const override;
		extent2d extent() const override;
		Window window() const override;
		RenderObjectHandle handle() const override;
		ETextureFormat render_format() const override;
		bool present() override;
		
		VkImageView back_buffer() const;
		
		VkSurfaceKHR surface = nullptr;
		VkSwapchainKHR swap_chain = nullptr;
		sakura::vector<VkImage> images;
		sakura::vector<VkImageView> image_views;

		VkSemaphore present_complete_semaphore;
		sakura::vector<VkSemaphore> render_to_screen_semaphores;
		
	protected:
		uint32_t image_index_ = 2;
		VkFormat format_;
		const SwapChainHandle handle_;
		const Window window_;
		extent2d extent_;
		const vk::RenderDevice& device_;
	};
}