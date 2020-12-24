#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura;
using namespace sakura::graphics;

sakura::graphics::vk::GpuSampler::GpuSampler(const GpuSamplerHandle handle, const vk::RenderDevice& dev,
	VkDevice device, VkPhysicalDevice physicalDevice, const SamplerDescriptor& desc) noexcept
	:handle_(handle), desc_(desc), owned_device_(device)
{
	auto createInfo = vk::translate(desc);
	if(vkCreateSampler(device, &createInfo, nullptr, &sampler))
	{
		sakura::error("[RenderGraphVulkan]: Failed to create sampler!");
	}
}

sakura::graphics::vk::GpuSampler::~GpuSampler()
{
	vkDestroySampler(owned_device_, sampler, nullptr);
}

sakura::graphics::RenderResourceHandle sakura::graphics::vk::GpuSampler::handle() const
{
	return handle_;
}

size_t sakura::graphics::vk::GpuSampler::size() const
{
	return size_t(-1);
}
