#include "RenderGraphVulkan/RenderGraphVulkan.h"

using namespace sakura::graphics;
using namespace sakura::graphics::vk;

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	sakura::error("failed to find suitable memory type!");
}

GpuBuffer::GpuBuffer(const GpuBufferHandle handle, const vk::RenderDevice& dev,
	VkDevice device, VkPhysicalDevice physicalDevice, const BufferDescriptor& desc) noexcept
	:owned_device_(device)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = desc.length;
	bufferInfo.usage = translate(desc.usages);
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer_) != VK_SUCCESS) 
	{
		sakura::error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer_, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &buffer_memory_) != VK_SUCCESS)
	{
		sakura::error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(device, buffer_, buffer_memory_, 0);

	void* data;
	vkMapMemory(device, buffer_memory_, 0, bufferInfo.size, 0, &data);
		memcpy(data, desc.data, (size_t)bufferInfo.size);
	vkUnmapMemory(device, buffer_memory_);
}

sakura::graphics::vk::GpuBuffer::~GpuBuffer()
{
	vkDestroyBuffer(owned_device_, buffer_, nullptr);
	vkFreeMemory(owned_device_, buffer_memory_, nullptr);
}

sakura::graphics::RenderResourceHandle sakura::graphics::vk::GpuBuffer::handle() const
{
	return handle_;
}

sakura::graphics::BufferUsages sakura::graphics::vk::GpuBuffer::usages() const
{
	return desc_.usages;
}

size_t sakura::graphics::vk::GpuBuffer::size() const
{
	return desc_.length;
}
