#pragma once
#include "RenderGraph/RenderGraphCore.h"
#include "vulkan/vulkan_core.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI GpuTexture : public IGpuTexture
	{
	public:
		GpuTexture(const GpuTextureHandle handle, const vk::RenderDevice& dev,
			VkDevice device, VkPhysicalDevice physicalDevice, const TextureDescriptor& desc) noexcept;
		~GpuTexture();

		RenderResourceHandle handle() const override;
		size_t size() const override;

		uint32 width() const override;
		uint32 height() const override;
		uint32 depth() const override;
		ETextureFormat render_format() const override;
		TextureUsages usages() const override;
		VkImage image = nullptr;
		VkImageView image_view = nullptr;
	protected:
		TextureDescriptor desc_;
		VkDevice owned_device_ = nullptr;
		GpuTextureHandle handle_ = GenerationalId::UNINITIALIZED;
	};
}
