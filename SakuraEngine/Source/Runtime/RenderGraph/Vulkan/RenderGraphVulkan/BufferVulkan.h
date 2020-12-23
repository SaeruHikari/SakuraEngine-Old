#pragma once
#include "RenderGraph/RenderGraphCore.h"
#include "vulkan/vulkan_core.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI GpuBuffer : public IGpuBuffer
	{
	public:
		GpuBuffer(const GpuBufferHandle handle, const vk::RenderDevice& dev, 
			VkDevice device, VkPhysicalDevice physicalDevice, const BufferDesc& desc) noexcept;
		~GpuBuffer();
		RenderResourceHandle handle() const override;
		size_t size() const override;

		BufferUsages usages() const override;

		VkBuffer buffer_ = VK_NULL_HANDLE;
		VkDeviceMemory buffer_memory_;
	
		VkDevice owned_device_ = VK_NULL_HANDLE;
	protected:
		BufferDesc desc_;
		GpuBufferHandle handle_;
	};
}
