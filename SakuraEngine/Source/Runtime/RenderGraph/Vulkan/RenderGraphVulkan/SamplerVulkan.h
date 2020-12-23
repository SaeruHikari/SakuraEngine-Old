#pragma once
#include "RenderGraph/RenderGraphCore.h"
#include "vulkan/vulkan_core.h"

namespace sakura::graphics::vk
{
	class RenderDevice;

	class RenderGraphVulkanAPI GpuSampler : public IGpuSampler
	{
	public:
		GpuSampler(const GpuSamplerHandle handle, const vk::RenderDevice& dev,
			VkDevice device, VkPhysicalDevice physicalDevice, const SamplerDesc& desc) noexcept;
		~GpuSampler();

		RenderResourceHandle handle() const override;
		size_t size() const override;

		VkSampler sampler = nullptr;
		
	protected:
		VkDevice owned_device_ = nullptr;
		SamplerDesc desc_;
		GpuSamplerHandle handle_ = GenerationalId::UNINITIALIZED;
	};
}
