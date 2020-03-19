/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 01:38:49
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-03-19 18:04:52
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
    class CommandContextVk : simplements CommandContext
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
            const Sakura::Graphics::GpuBuffer& vb) override final;
        virtual void BindIndexBuffers(
            const Sakura::Graphics::GpuBuffer& ib) override final;

        virtual void BindRootArguments(const PipelineBindPoint bindPoint,
            const RootArgument** arguments, uint32_t argumentNum) override final;

        virtual void CopyResource(GpuBuffer& src, GpuBuffer& dst,
            const uint64_t size,
            const uint64_t srcOffset = 0, const uint64_t dstOffset = 0) override final;
        
        virtual void CopyResource(GpuBuffer& src, GpuTexture& dst,
            const uint32_t imageWidth, const uint32_t imageHeight,
            const ImageAspectFlags aspectFlags, const uint64_t srcOffset = 0) override final;

        virtual void CopyResource(GpuBuffer& src, GpuTexture& dst,
            const BufferImageCopy& info) override final;

        virtual void ResourceBarrier(const GpuBuffer& toTransition) override final;
    public:
        VkCommandBuffer commandBuffer;
    protected:
        CommandContextVk(const CGD_Vk& _cgd, ECommandType type,
            bool bTransiant = false);
    protected:
        const CGD_Vk& cgd;
        GraphicsPipelineVk* vkGp = nullptr;
        VkCommandPool commandPool;
        VkFence recordingFence = VK_NULL_HANDLE;
    };
} // namespace Sakura::Graphics

