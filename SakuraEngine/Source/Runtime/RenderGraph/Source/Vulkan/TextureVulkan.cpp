#include "RenderGraphVulkan/RenderGraphVulkan.h"

sakura::graphics::vk::GpuTexture::GpuTexture(const GpuTextureHandle handle, const vk::RenderDevice& dev,
    VkDevice device, VkPhysicalDevice physicalDevice, const TextureDescriptor& desc) noexcept
	:handle_(handle), desc_(desc), owned_device_(device)
{
	auto createInfo = vk::translate(desc);
	if(vkCreateImage(device, &createInfo, nullptr, &image))
	{
		sakura::error("[RenderGraphVulkan]: Failed to create texture!");
	}

	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	// TODO: Try to support VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT  
	// TODO: Try to support VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT   
	viewInfo.flags = 0;
	viewInfo.format = createInfo.format;
	viewInfo.components.r = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R;
	viewInfo.components.g = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G;
	viewInfo.components.b = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B;
	viewInfo.components.a = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A;
	viewInfo.image = image;
	viewInfo.viewType = match(desc.dimension);
	VkImageSubresourceRange range = {};
	range.baseMipLevel = 0;
	range.baseArrayLayer = 0;
	range.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
	range.layerCount = desc.array_layers;
	range.levelCount = desc.mip_levels;
	if(vkCreateImageView(device, &viewInfo, nullptr, &image_view))
	{
		sakura::error("[RenderGraphVulkan]: Failed to create texture view!");
	}
}

sakura::graphics::vk::GpuTexture::~GpuTexture()
{
	vkDestroyImage(owned_device_, image, nullptr);
	vkDestroyImageView(owned_device_, image_view, nullptr);
}

sakura::graphics::RenderResourceHandle sakura::graphics::vk::GpuTexture::handle() const
{
	return handle_;
}

size_t sakura::graphics::vk::GpuTexture::size() const
{
	sakura::error("UNIMPLEMENTED!");
	return 0;
}

sakura::uint32 sakura::graphics::vk::GpuTexture::width() const
{
	return desc_.size.width;
}

sakura::uint32 sakura::graphics::vk::GpuTexture::height() const
{
	return desc_.size.height;
}

sakura::uint32 sakura::graphics::vk::GpuTexture::depth() const
{
	return desc_.size.depth;
}

sakura::graphics::ETextureFormat sakura::graphics::vk::GpuTexture::render_format() const
{
	return desc_.format;
}

sakura::graphics::TextureUsages sakura::graphics::vk::GpuTexture::usages() const
{
	return desc_.usages;
}
