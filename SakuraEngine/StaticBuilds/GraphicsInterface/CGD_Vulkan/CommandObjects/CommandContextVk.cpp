/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Vulkan implementation of CommandContext
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:25:06
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-23 13:02:48
 */
#include "../../GraphicsCommon/CommandObjects/CommandContext.h"
#include "../CGD_Vulkan.h"
#include "CommandContextVk.h"
#include "Core/EngineUtils/log.h"
#include "../GraphicsObjects/GraphicsPipelineVk.h"
#include "../GraphicsObjects/ComputePipelineVk.h"
#include "../CommandObjects/CommandQueueVk.h"
#include "../GraphicsObjects/RootSignatureVk.h"
#include "../ResourceObjects/GpuResourceVk.h"
#include <set>

#ifndef PROFILING_POOL
//#define PROFILING_POOL 
#endif

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

CommandContext* CGD_Vk::AllocateContext(ECommandType type, bool bTransiant)
{
    std::lock_guard<std::mutex> LockGurad(contextAllocationMutex);
#ifdef PROFILING_POOL
    for (auto i = 0; i < contextPools[type].size(); i++)
    {
        std::cout << "context no." << i << ": " << 
            ((vkGetFenceStatus(entityVk.device,
            ((CommandContextVk*)contextPools[type][i].get())->recordingFence) == VK_SUCCESS) ? "free    " : "executing    ") << std::endl;
    }
#endif
    if(!availableContexts[type].empty())
    {
        auto res = availableContexts[type].front();
        if(vkGetFenceStatus(entityVk.device, 
            ((CommandContextVk*)res)->recordingFence) == VK_SUCCESS)
        {
            availableContexts[type].pop();
            return res;
        }
    }
    CommandContext* newContext = new CommandContextVk(*this, type, bTransiant);
    auto result = std::unique_ptr<CommandContext>(newContext);
    auto ptr = result.get();
    contextPools[type].push_back(std::move(result));
    return ptr;
}

void CGD_Vk::FreeAllContexts(ECommandType type)
{
    std::lock_guard<std::mutex> LockGurad(contextAllocationMutex);
    for(auto i = 0; i < contextPools[type].size(); i++)
    {
        availableContexts[type].push(contextPools[type][i].get());
    }
}

void CGD_Vk::FreeContext(CommandContext* context)
{
    std::lock_guard<std::mutex> LockGurad(contextAllocationMutex);
    auto vkContext = (CommandContextVk*)context;
    availableContexts[context->GetCommandContextType()].push(context);
}

CommandContextVk::CommandContextVk(const CGD_Vk& _cgd, 
    ECommandType type, bool bTransiant)
    : cgd(_cgd)
{
    auto indices = _cgd.GetQueueFamily().graphicsFamily.value();
    switch (type)
    {
    case ECommandType::CommandContext_Graphics:
        indices = _cgd.GetQueueFamily().graphicsFamily.value();
        break;
    case ECommandType::CommandContext_Compute:
        indices = _cgd.GetQueueFamily().computeFamily.value();
        break;
    case ECommandType::CommandContext_Copy:
        indices = _cgd.GetQueueFamily().copyFamily.value();
        break;
    default:
        break;
    }
    this->m_Type = type;
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = indices;
    poolInfo.flags = bTransiant ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0; 
    if (vkCreateCommandPool(_cgd.GetCGDEntity().device, &poolInfo,
        nullptr, &commandPool) != VK_SUCCESS) 
    {
        CGD_Vk::error("Vulkan: failed to create command pool!");
        throw std::runtime_error("failed to create command pool!");
    }

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(_cgd.GetCGDEntity().device, &allocInfo,
        &commandBuffer) != VK_SUCCESS) 
    {
        CGD_Vk::error("Vulkan: failed to allocate command buffers!");
        throw std::runtime_error("failed to allocate command buffers!");
    }

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if(vkCreateFence(_cgd.GetCGDEntity().device, &fenceInfo,
        nullptr, &recordingFence) != VK_SUCCESS)
    {        
        CGD_Vk::error("failed to create synchronization objects for a CmdBuffer!");
        throw std::runtime_error("failed to create synchronization objects for a CmdBuffer!");
    }
}

CommandContextVk::~CommandContextVk()
{
    vkWaitForFences(cgd.GetCGDEntity().device,
        1, &recordingFence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(cgd.GetCGDEntity().device, recordingFence, nullptr);
    vkDestroyCommandPool(cgd.GetCGDEntity().device, commandPool, nullptr);
}

void CommandContextVk::Begin()
{
    bOpen = true;
    vkResetCommandPool(cgd.GetCGDEntity().device,
        commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT); 
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
    {
        CGD_Vk::error("Vulkan: failed to begin recording command buffer!");
        throw std::runtime_error("failed to begin recording command buffer!");
    }
}

void CommandContextVk::BindVertexBuffers(const GpuBuffer& vb) 
{
    VkDeviceSize offsets[] = {0};
    VkBuffer bufs[] = {((const GpuResourceVkBuffer&)vb).buffer};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, bufs, offsets);
}

void CommandContextVk::BindIndexBuffers(const GpuBuffer& ib)
{
    VkBuffer buf = ((const GpuResourceVkBuffer&)ib).buffer;
    vkCmdBindIndexBuffer(commandBuffer, buf, 0, VK_INDEX_TYPE_UINT32);
}

void CommandContextVk::BeginComputePass(ComputePipeline* cp)
{
    vkCp = (ComputePipelineVk*)cp;
    if(vkCp->pipeline == VK_NULL_HANDLE)
    {
        CGD_Vk::error("CGD: please bind VkPipeline first!");
        throw std::runtime_error("CGD: please bind VkPipeline first!");
    }
    vkCmdBindPipeline(commandBuffer,
        VK_PIPELINE_BIND_POINT_COMPUTE, ((ComputePipelineVk*)cp)->pipeline);
}

void CommandContextVk::DispatchCompute(uint32 groupCountX,
    uint32 groupCountY, uint32 groupCountZ)
{
    vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void CommandContextVk::BeginRenderPass(
    GraphicsPipeline* gp, const RenderTargetSet& rts)
{
    vkGp = (GraphicsPipelineVk*)gp;
    if(vkGp->graphicsPipeline == VK_NULL_HANDLE)
    {
        CGD_Vk::error("CGD: please bind VkPipeline first!");
        throw std::runtime_error("CGD: please bind VkPipeline first!");
    }
    vkCmdBindPipeline(commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS, vkGp->graphicsPipeline);
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = vkGp->progress.renderPass;
    renderPassInfo.framebuffer = vkGp->FindFrameBuffer(rts);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent =
        Transfer(rts.rts[0].resource->GetExtent());

    std::vector<VkClearValue> clearValues(rts.rtNum);
    for(auto i = 0; i < rts.rtNum; i++)
    {
        clearValues[i] = *(VkClearValue*)&(rts.rts[i].clearValue);
    }
    
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    vkCmdBeginRenderPass(commandBuffer, 
        &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandContextVk::BindRootArguments(const PipelineBindPoint bindPoint,
    const RootArgument** arguments, uint32_t argumentNum)
{
    std::vector<VkDescriptorSet> descriptorSets(argumentNum);
    for(auto i = 0; i < argumentNum; i++)
    {
        descriptorSets[i] = 
            (((const RootArgumentVk**)arguments)[i])->descriptorSet;
    }
    if(bindPoint == PipelineBindPoint::BindPointGraphics)
    vkCmdBindDescriptorSets(commandBuffer, Transfer(bindPoint),
        vkGp->pipelineLayout, 0, argumentNum, descriptorSets.data(), 0, nullptr);
    else if(bindPoint == PipelineBindPoint::BindPointCompute)
    vkCmdBindDescriptorSets(commandBuffer, Transfer(bindPoint),
        vkCp->pipelineLayout, 0, argumentNum, descriptorSets.data(), 0, nullptr);
}

void CommandContextVk::Draw(uint32 vertexCount, uint32 instanceCount,
    uint32 firstVertex, uint32 firstInstance)
{
    vkCmdDraw(commandBuffer, vertexCount,
        instanceCount, firstVertex, firstInstance);
}

void CommandContextVk::DrawIndexed(const uint32_t indicesCount,
    const uint32_t instanceCount)
{
    vkCmdDrawIndexed(commandBuffer, indicesCount,
        instanceCount, 0, 0, 0);
}

void CommandContextVk::CopyResource(GpuBuffer& src, GpuBuffer& dst,
    const uint64 size, const uint64 srcOffset, const uint64 dstOffset)
{
    VkBufferCopy copyRegion = {};
    copyRegion.dstOffset = dstOffset;
    copyRegion.srcOffset = srcOffset;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer,
        ((GpuResourceVkBuffer&)src).buffer,
        ((GpuResourceVkBuffer&)dst).buffer, 1, &copyRegion);
}

void CommandContextVk::CopyResource(GpuBuffer& src, GpuTexture& dst,
    const BufferImageCopy& info)
{
    VkBufferImageCopy region = {};
    region.bufferOffset = info.bufferOffset;
    region.bufferRowLength = info.bufferRowLength;
    region.bufferImageHeight = info.bufferImageHeight;
    region.imageSubresource.aspectMask = info.imageSubresource.aspectMask;
    region.imageSubresource.mipLevel = info.imageSubresource.mipLevel;
    region.imageSubresource.baseArrayLayer = info.imageSubresource.baseArrayLayer;
    region.imageSubresource.layerCount = info.imageSubresource.layerCount;
    region.imageOffset = *(VkOffset3D*)&info.imageOffset;
    region.imageExtent = *(VkExtent3D*)&info.imageExtent;
    vkCmdCopyBufferToImage(commandBuffer,
        ((GpuResourceVkBuffer&)src).buffer,
        ((GpuResourceVkImage&)dst).image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void CommandContextVk::CopyResource(GpuBuffer& src, GpuTexture& dst,
        const uint32_t imageWidth, const uint32_t imageHeight,
        const ImageAspectFlags aspectFlags, const uint64_t srcOffset)
{
    VkBufferImageCopy region = {};
    region.bufferOffset = srcOffset;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = aspectFlags;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        imageWidth,
        imageHeight,
        1
    };
    vkCmdCopyBufferToImage(commandBuffer,
        ((GpuResourceVkBuffer&)src).buffer,
        ((GpuResourceVkImage&)dst).image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void CommandContextVk::ResourceBarrier(GpuBuffer& buffer)
{
    
}

void CommandContextVk::ResourceBarrier(GpuTexture& texture,
    const ImageLayout oldLayout, const ImageLayout newLayout,
    const TextureSubresourceRange& range)
{
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = *(VkImageLayout*)&oldLayout;
    barrier.newLayout = *(VkImageLayout*)&newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = ((const GpuResourceVkImage&)texture).image;
    barrier.subresourceRange.aspectMask = range.aspectMask;
    barrier.subresourceRange.baseMipLevel = range.baseMipLevel;
    barrier.subresourceRange.levelCount = range.mipLevels;
    barrier.subresourceRange.baseArrayLayer = range.baseArrayLayer;
    barrier.subresourceRange.layerCount = range.layerCount;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED 
            && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } 
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL 
        && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else 
    {
        CGD_Vk::error("unsupported layout transition!");
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
}

void CommandContextVk::GenerateMipmaps(GpuTexture& texture, Format format,
    uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels)
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(
        cgd.GetCGDEntity().physicalDevice, Transfer(format), &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) 
    {
        CGD_Vk::error("texture image format does not support linear blitting!");
		throw std::runtime_error("texture image format does not support linear blitting!");
	}

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = ((GpuResourceVkImage&)texture).image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit = {};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
            ((GpuResourceVkImage&)texture).image, 
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            ((GpuResourceVkImage&)texture).image, 
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);
}

void CommandContextVk::Reset()
{
    vkResetCommandPool(cgd.GetCGDEntity().device,
        commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT); 
    if(bOpen)
        End();
}

void CommandContextVk::EndRenderPass()
{
    vkCmdEndRenderPass(commandBuffer);
}

void CommandContextVk::End()
{
    bOpen = false;
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
    {
        CGD_Vk::error("Vulkan: failed to record command buffer!");
        throw std::runtime_error("failed to record command buffer!");
    }
}
