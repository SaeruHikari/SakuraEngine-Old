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

GPUBuffer::GPUBuffer(const RenderBufferHandle handle, const vk::RenderDevice& dev,
	VkDevice device, VkPhysicalDevice physicalDevice, const BufferDesc& desc) noexcept
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = desc.length;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
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

sakura::graphics::vk::GPUBuffer::~GPUBuffer()
{
	vkDestroyBuffer(owned_device_, buffer_, nullptr);
	vkFreeMemory(owned_device_, buffer_memory_, nullptr);
}

sakura::graphics::RenderResourceHandle sakura::graphics::vk::GPUBuffer::handle() const
{
	return handle_;
}

sakura::graphics::EBufferUsage sakura::graphics::vk::GPUBuffer::usage() const
{
	return desc_.usage;
}

size_t sakura::graphics::vk::GPUBuffer::size() const
{
	return desc_.length;
}

sakura::graphics::EBufferOptions sakura::graphics::vk::GPUBuffer::options() const
{
	return desc_.options;
}

