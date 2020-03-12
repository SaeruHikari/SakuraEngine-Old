/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Vulkan implementation of CommandContext
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:25:06
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-12 00:32:27
 */
#include "../../GraphicsCommon/CommandObjects/CommandContext.h"
#include "../CGD_Vulkan.h"
#include "CommandContextVk.h"
#include "Core/EngineUtils/log.h"
#include "../GraphicsObjects/GraphicsPipelineVk.h"
#include "../CommandObjects/CommandQueueVk.h"
#include <set>

#ifndef PROFILING_POOL
#define PROFILING_POOL 
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
    auto graphicsQueue = _cgd.GetCGDEntity().graphicsQueue->vkQueue;
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

void CommandContextVk::Begin(GraphicsPipeline* gp)
{
    vkGp = (GraphicsPipelineVk*)gp;
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkResetCommandPool(cgd.GetCGDEntity().device,
        commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT); 
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
    {
        CGD_Vk::error("Vulkan: failed to begin recording command buffer!");
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    if(vkGp->graphicsPipeline == VK_NULL_HANDLE)
    {
        CGD_Vk::error("CGD: please bind VkPipeline first!");
        throw std::runtime_error("CGD: please bind VkPipeline first!");
    }
    vkCmdBindPipeline(commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS, vkGp->graphicsPipeline);
}

void CommandContextVk::SetRenderTargets(const RenderTargetSet& rts)
{
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = vkGp->progress.renderPass;
    renderPassInfo.framebuffer = vkGp->FindFrameBuffer(rts);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent =
        Transfer(rts.rts[0].resource->GetExtent());

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;
    vkCmdBeginRenderPass(commandBuffer, 
        &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandContextVk::Draw(uint32 vertexCount, uint32 instanceCount,
    uint32 firstVertex, uint32 firstInstance)
{
    vkCmdDraw(commandBuffer, vertexCount,
        instanceCount, firstVertex, firstInstance);
}

void CommandContextVk::End()
{
    vkCmdEndRenderPass(commandBuffer);
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
    {
        CGD_Vk::error("Vulkan: failed to record command buffer!");
        throw std::runtime_error("failed to record command buffer!");
    }
}
