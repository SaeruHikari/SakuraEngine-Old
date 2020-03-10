/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:38:49
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-10 11:22:46
 */
#pragma once
#include "../../GraphicsCommon/CommandObjects/CommandContext.h"
#include "vulkan/vulkan.h"

namespace Sakura::Graphics::Vk
{
    class CommandContextVk : SImplements CommandContext
    {
        friend class CGD_Vk;

        virtual ~CommandContextVk() override final;
    public:
        virtual void Begin(const GraphicsPipeline* gp) override final;
        virtual void End() override final;
        virtual void Draw(uint32 vertexCount, uint32 instanceCount,
            uint32 firstVertex, uint32 firstInstance) override final;
    protected:
        CommandContextVk(const CGD_Vk& _cgd, ECommandType type,
            bool bTransiant = false);
    protected:
        const CGD_Vk& cgd;
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
    };
} // namespace Sakura::Graphics

