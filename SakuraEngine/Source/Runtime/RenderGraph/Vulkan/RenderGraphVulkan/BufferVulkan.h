#pragma once
#include "RenderGraph/RenderGraphCore.h"
#include "vulkan/vulkan_core.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI GPUBuffer : public IGPUBuffer
	{
	public:
		GPUBuffer(const RenderBufferHandle handle, const vk::RenderDevice& dev, 
			VkDevice device, VkPhysicalDevice physicalDevice, const BufferDesc& desc) noexcept;
		~GPUBuffer();
		RenderResourceHandle handle() const override;
		size_t size() const override;

		EBufferUsage usage() const override;
		EBufferOptions options() const override;

		VkBuffer buffer_ = VK_NULL_HANDLE;
		VkDeviceMemory buffer_memory_;
	
		VkDevice owned_device_ = VK_NULL_HANDLE;
	protected:
		BufferDesc desc_;
		RenderBufferHandle handle_;
	};
}
