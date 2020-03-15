/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:38:49
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-15 21:14:38
 */
#pragma once
#include "../../GraphicsCommon/CommandObjects/CommandContext.h"
#include "vulkan/vulkan.h"

namespace Sakura::Graphics::Vk
{
    class GraphicsPipelineVk;
}


namespace Sakura::Graphics::Vk
{
    class CommandContextVk : SImplements CommandContext
    {
        friend class CGD_Vk;
        friend class CommandQueueVk;
        virtual ~CommandContextVk() override final;
    public:
        virtual void Begin() override final;
        virtual void End() override final;
        virtual void Reset() override final;
        virtual void EndRenderPass() override final;

        virtual void BeginRenderPass(
            GraphicsPipeline* gp, const RenderTargetSet& rts) override final;

        virtual void Draw(uint32 vertexCount, uint32 instanceCount,
            uint32 firstVertex, uint32 firstInstance) override final;
        virtual void DrawIndexed(const uint32_t indicesCount,
            const uint32_t instanceCount) override final;


        virtual void BindVertexBuffers(
            const Sakura::Graphics::GpuResource& vb) override final;
        virtual void BindIndexBuffers(
            const Sakura::Graphics::GpuResource& ib) override final;
        
        virtual void CopyBuffer(GpuResource& src, GpuResource& dst,
            const uint64_t srcOffset,
            const uint64_t dstOffset = 0, const uint64_t size = 0) override final;
    protected:
        CommandContextVk(const CGD_Vk& _cgd, ECommandType type,
            bool bTransiant = false);
    protected:
        const CGD_Vk& cgd;
        GraphicsPipelineVk* vkGp = nullptr;
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
        VkFence recordingFence = VK_NULL_HANDLE;
    };
} // namespace Sakura::Graphics

