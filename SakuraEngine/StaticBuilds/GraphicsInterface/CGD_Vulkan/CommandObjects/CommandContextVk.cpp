/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Vulkan implementation of CommandContext
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:25:06
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-10 01:00:46
 */
#include "../../GraphicsCommon/CommandObjects/CommandContext.h"
#include "../CGD_Vulkan.h"
#include "CommandContextVk.h"
#include "Core/EngineUtils/log.h"
#include "../GraphicsObjects/GraphicsPipelineVk.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

CommandContext* CGD_Vk::AllocateContext(ECommandType type, bool bTransiant)
{
    std::lock_guard<std::mutex> LockGurad(contextAllocationMutex);
    if(!availableContexts[type].empty())
    {
        auto res = availableContexts[type].front();
        availableContexts[type].pop();
        return res;
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
    availableContexts[context->GetCommandContextType()].push(context);
}

CommandContextVk::CommandContextVk(const CGD_Vk& _cgd, 
    ECommandType type, bool bTransiant)
    : cgd(_cgd)
{
    auto indices = _cgd.GetQueueFamily().graphicsFamily.value();
    
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
}

CommandContextVk::~CommandContextVk()
{
    vkDestroyCommandPool(cgd.GetCGDEntity().device, commandPool, nullptr);
}

void CommandContextVk::Begin()
{
    VkCommandBufferBeginInfo beginInfo = {};
    VkClearValue clearColor;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) 
    {
        CGD_Vk::error("Vulkan: failed to begin recording command buffer!");
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    auto gp = (GraphicsPipelineVk*)cgd.GetActiveGP();
    if(gp == nullptr || gp->graphicsPipeline == VK_NULL_HANDLE)
    {
        CGD_Vk::error("CGD: please set Pipeline first!");
        throw std::runtime_error("CGD: please set Pipeline first!");
    }
    vkCmdBindPipeline(commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS, gp->graphicsPipeline);
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

void CommandContextVk::End()
{
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) 
    {
        CGD_Vk::error("Vulkan: failed to record command buffer!");
        throw std::runtime_error("failed to record command buffer!");
    }
}
