/*
 * @CopyRight: MIT License
 * Copyright (c) 2020 SaeruHikari
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THESOFTWARE.
 * 
 * 
 * @Description: 
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-29 11:46:00
 * @LastEditTime: 2020-03-24 12:21:50
 */
#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SakuraEngine/StaticBuilds/PixelOperators/ImageUtils.h"
#include "SakuraEngine/StaticBuilds/Graphicsinterface/GraphicsCommon/CGD.h"
#include "SakuraEngine/StaticBuilds/Graphicsinterface/CGD_Vulkan/CGD_Vulkan.h"
extern "C"
{
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_surface.h"
	#include "SDL2/SDL_vulkan.h"
	#undef main
}
#include "SakuraEngine/Core/Core.h"
#include "vulkan/vulkan.h"
#include "Extern/include/SDL2Tools/SDL2Vk.hpp"
#include "SakuraEngine/StaticBuilds/Graphicsinterface/CGD_Vulkan/Flags/FormatVk.h"
#include "SakuraEngine/StaticBuilds/Graphicsinterface/CGD_Vulkan/Flags/GraphicsPipelineStatesVk.h"
#include "SakuraEngine/StaticBuilds/TaskSystem/TaskSystem.h"
#include "SakuraEngine/StaticBuilds/Graphicsinterface/CGD_Vulkan/GraphicsObjects/FenceVk.h"
#include "SakuraEngine/StaticBuilds/ImGuiProfiler/ImGuiProfiler.hpp"
#include "SakuraEngine/StaticBuilds/GraphicsInterface/GraphicsCommon/GraphicsObjects/ComputePipeline.h"

using namespace Sakura;
using namespace Sakura::Graphics::Vk;

#if defined(CONFINFO_PLATFORM_LINUX) 
#elif defined(CONFINFO_PLATFORM_MACOS)
    Sakura::fs::file vs_f
    ("/Users/saeruhikari/Coding/SakuraEngine/SakuraTestProject/shaders/VertexBuffer/HWVert.spv",
        'r');
    Sakura::fs::file fs_f
    ("/Users/saeruhikari/Coding/SakuraEngine/SakuraEngine/UnitTests/UnitTestGraphics/frag.spv",
        'r');
#elif defined(CONFINFO_PLATFORM_WIN32)
    Sakura::fs::file vs_srv
    ("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\SRV\\SRVVertex.spv",
        'r');  
    Sakura::fs::file fs_srv
    ("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\SRV\\SRVPixel.spv",
        'r');
    Sakura::fs::file cs_avgfilter
	("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\Compute\\avgfiltering.comp.spv",
		'r');
    const std::string texPath =
        "D:\\Coding\\SakuraEngine\\SakuraTestProject\\textures\\CGBull.jpg";
#endif

struct Vertex
{
    Sakura::Math::Vector3f pos;
    Sakura::Math::Vector3f color;
    glm::vec2 texCoord;
    static VertexInputBindingDescription getBindingDescription() {
        VertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VertexInputRate::VertexInputRateVertex;

        return bindingDescription;
    }

    static std::array<VertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VertexInputAttributeDescription, 3> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = Format::R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = Format::R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
    
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = Format::R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};
const std::vector<Vertex> vertices = {
{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};
const std::vector<uint32_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};

struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class VkTestApplication
{
public:
    void run()
    {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        win = VkSDL_CreateWindow("SakuraEngine Window: CGD Vulkan", 1280, 720);
        initVulkan();
        int run = 1;
        while (run)
        {
            SDL_Event evt;
            while (SDL_PollEvent(&evt))
            {
                switch (evt.type)
                {
                case SDL_QUIT:
                    run = 0; break;
                case SDL_WINDOWEVENT:
					if (evt.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
						std::cout << "MESSAGE:Resizing window...\n";
                        ResizeWindow(evt.window.data1, evt.window.data2);
					}
                default:
                    break;
                }
            }
            mainLoop();
        }
        cleanUp();
    }

private:
    void createShader()
    {
		// Create PSO vs_cbv
		std::vector<char> vs_bytes(vs_srv.size());
		std::vector<char> fs_bytes(fs_srv.size());
        std::vector<char> cs_bytes(cs_avgfilter.size());
		vs_srv.read(vs_bytes.data(), vs_bytes.size());
		fs_srv.read(fs_bytes.data(), fs_bytes.size());
		cs_avgfilter.read(cs_bytes.data(), cs_bytes.size());
		vertshader = cgd->CreateShader(vs_bytes.data(), vs_bytes.size());
		fragshader = cgd->CreateShader(fs_bytes.data(), fs_bytes.size());
        computeshader = cgd->CreateShader(cs_bytes.data(), cs_bytes.size());
        // shader stages
		vsStage.stage = ShaderStageFlags::VertexStage;
		vsStage.shader = vertshader.get(); vsStage.entry = "main";
		fsStage.stage = ShaderStageFlags::PixelStage;
		fsStage.shader = fragshader.get(); fsStage.entry = "main";
        csStage.stage = ShaderStageFlags::ComputeStage;
        csStage.shader = computeshader.get(); csStage.entry = "main";
    }

    void ResizeWindow(uint32 width, uint32 height)
    {
        cgd->WaitIdle();   
        // recreate swapchain
        swapChain.reset();
        swapChain.reset(cgd->CreateSwapChain(width, height, &surface));
        createDepth(); 
        // Create Graphics Pipeline
        DepthStencilStateCreateInfo depthStencil;
        GraphicsPipelineCreateInfo info;
        info.PushShaderStage(vsStage, fsStage);
        info.AddViewport(width, height);
        info.AddScissor(swapChain->GetExtent());
        info.rootSignature = rootSignature.get();
        info.depthStencilCreateInfo = depthStencil;
		info.AddVertexBinding(Vertex::getBindingDescription());
		info.AddVertexAttribute(Vertex::getAttributeDescriptions());

		// Create Render Pass
		RenderPassCreateInfo rpinfo;
        AttachmentDescription colorAttachment;
		colorAttachment.format = swapChain->GetPixelFormat();
        AttachmentDescription depthAttachment;
        depthAttachment.format = cgd->FindDepthFormat();
        depthAttachment.storeOp = AttachmentStoreOp::AttachmentStoreOpDontCare;
        depthAttachment.finalLayout = ImageLayout::DepthStencilAttachment;
        rpinfo.AttachColor(colorAttachment);
        rpinfo.AttachDepth(depthAttachment);
		
		pass.reset(cgd->CreateRenderPass(rpinfo));
		Pipeline.reset(cgd->CreateGraphicsPipeline(info, *pass.get()));

        profiler.reset();
        profiler = std::make_unique<Sakura::Graphics::Im::ImGuiProfiler>(*cgd.get());
        profiler->ImGuiInitialize(win, swapChain.get()); 
    }

    void createTexture()
    {
        int width, height, channels;
        auto pixels = Sakura::Images::LoadFromDisk<std::byte>
            (texPath.c_str(), width, height, channels);
        std::unique_ptr<GpuBuffer> uploadBuffer;
        uploadBuffer.reset(cgd->CreateUploadBuffer(width * height * 4));
        uploadBuffer->UpdateValue([&](void* ptr) -> void
            {
                memcpy(ptr, pixels, static_cast<size_t>(width * height * 4));
            });

        auto mipLevels = Sakura::Graphics::CalculateMipLevels(width, height);
        texture.reset(
            cgd->CreateGpuTexture(Format::R8G8B8A8_UNORM, width, height,
            ImageUsage::TransferDstImage | ImageUsage::SampledImage |
            ImageUsage::TransferSrcImage | ImageUsage::StorageImage
            , mipLevels));
        textureTarget.reset(
            cgd->CreateGpuTexture(Format::R8G8B8A8_UNORM, width, height,
            ImageUsage::SampledImage | ImageUsage::StorageImage,
            mipLevels));

        TextureSubresourceRange textureSubresource;
        textureSubresource.mipLevels = mipLevels;
        std::unique_ptr<CommandContext> context;
        context.reset(cgd->CreateContext(ECommandType::CommandContext_Graphics));
        context->Begin();
        context->ResourceBarrier(*texture.get(),
            ImageLayout::Unknown, ImageLayout::TransferDstOptimal,
            textureSubresource);
        context->CopyResource(*uploadBuffer.get(), *texture.get(),
            width, height, ImageAspectFlag::ImageAspectColor);
        context->GenerateMipmaps(*texture.get(), Format::R8G8B8A8_UNORM,
            width, height, mipLevels);
        context->ResourceBarrier(*texture.get(),
            ImageLayout::Unknown, ImageLayout::UAV, textureSubresource);
        context->ResourceBarrier(*textureTarget.get(),
            ImageLayout::Unknown, ImageLayout::UAV, textureSubresource);
        context->End();
        cgd->GetGraphicsQueue()->Submit(context.get());
        cgd->GetGraphicsQueue()->WaitIdle();

        textureView.reset(
            cgd->ViewIntoTexture(*texture.get(), Format::R8G8B8A8_UNORM,
                ImageAspectFlag::ImageAspectColor));
        textureTargetView.reset(
           cgd->ViewIntoTexture(*textureTarget.get(), Format::R8G8B8A8_UNORM,
                ImageAspectFlag::ImageAspectColor));

        SamplerCreateInfo samplerInfo;
        samplerInfo.mipmapMode = SamplerMipmapMode::SamplerMipmapModeLinear;
        samplerInfo.minLod = 0;
        samplerInfo.maxLod = mipLevels;
        samplerInfo.mipLodBias = 0;
        sampler.reset(cgd->CreateSampler(samplerInfo));

        RootArgumentAttachment compAttachments[2];
        TexSamplerAttachment srcAttach;
        srcAttach.imageView = textureView.get();
        srcAttach.sampler = sampler.get();
        srcAttach.imageLayout = ImageLayout::UAV;
        compAttachments[0].info = srcAttach;
        compAttachments[0].rootArgType = SignatureSlotType::StorageImageSlot;
        compAttachments[0].dstBinding = 0;
        TexSamplerAttachment dstAttach;
        dstAttach.imageView = textureTargetView.get();
        dstAttach.sampler = sampler.get();
        dstAttach.imageLayout = ImageLayout::UAV;
        compAttachments[1].info = dstAttach;
        compAttachments[1].rootArgType = SignatureSlotType::StorageImageSlot;
        compAttachments[1].dstBinding = 1;
        compArgument->UpdateArgument(compAttachments, 2);

        context.reset(cgd->CreateContext(ECommandType::CommandContext_Graphics));
        context->Begin();
        context->BeginComputePass(compPipeline.get());
        const auto* cmparg = compArgument.get();
        context->BindRootArguments(PipelineBindPoint::BindPointCompute,
            &cmparg, 1);
        context->DispatchCompute(
            texture->GetExtent().width / 16, 
            texture->GetExtent().height / 16, 1);
        context->ResourceBarrier(*textureTarget.get(),
            ImageLayout::UAV, ImageLayout::ShaderReadOnlyOptimal,
            textureSubresource);
        context->End();
        cgd->GetGraphicsQueue()->Submit(context.get());
        cgd->WaitIdle();
    }
    
    void createDepth()
    {
        auto depthFormat = cgd->FindDepthFormat();
        auto extent = swapChain->GetExtent();
        depth.reset(
            cgd->CreateGpuTexture(depthFormat, extent.width, extent.height,
            ImageUsage::DepthStencilAttachmentImage | ImageUsage::SampledImage));
        depthView.reset(
            cgd->ViewIntoTexture(*depth.get(), depthFormat, ImageAspectDepth));
    }

    void createBuffer()
    {
        auto vbsize = sizeof(Vertex) * vertices.size();
        auto ibsize = sizeof(uint32_t) * indices.size();
        vertexBuffer.reset(cgd->CreateGpuBuffer(vbsize,
            BufferUsage::VertexBuffer | BufferUsage::TransferDstBuffer,
            CPUAccessFlags::None));
        indexBuffer.reset(cgd->CreateGpuBuffer(ibsize,
            BufferUsage::IndexBuffer | BufferUsage::TransferDstBuffer,
            CPUAccessFlags::None));
        std::unique_ptr<GpuBuffer> uploadBufferVB, uploadBufferIB;
        uploadBufferVB.reset(cgd->CreateUploadBuffer(vbsize));
        uploadBufferIB.reset(cgd->CreateUploadBuffer(ibsize));
        uploadBufferVB->UpdateValue([&](void* ptr) -> void
            {
                memcpy(ptr, vertices.data(), vbsize);
            });
        uploadBufferIB->UpdateValue([&](void* ptr) -> void
            {
                memcpy(ptr, indices.data(), ibsize);
            });

        std::unique_ptr<CommandContext> context; 
        context.reset(cgd->CreateContext(ECommandType::CommandContext_Copy));
        context->Begin();
        context->CopyResource(*uploadBufferVB.get(), *vertexBuffer.get(),vbsize);
        context->CopyResource(*uploadBufferIB.get(), *indexBuffer.get(), ibsize);
        context->End();
        
        cgd->GetCopyQueue()->Submit(context.get());
        cgd->GetCopyQueue()->WaitIdle();

        constantBuffer.reset(
            cgd->CreateGpuBuffer(sizeof(UniformBufferObject),
            BufferUsage::ConstantBuffer, CPUAccessFlags::ReadWrite));
        createTexture();
        createDepth();
    }

    void createRootSignature()
    {
        RootSignatureCreateInfo info = {};
        std::vector<SignatureSlot> slots(3);
        slots[0].type = SignatureSlotType::UniformBufferSlot;
        slots[0].stageFlags = ShaderStageFlags::VertexStage;
        slots[1].type = SignatureSlotType::SamplerSlot;
        slots[1].stageFlags = ShaderStageFlags::PixelStage;
        slots[2].type = SignatureSlotType::SampledImageSlot;
        slots[2].stageFlags = ShaderStageFlags::PixelStage;
        
        SamplerCreateInfo samplerInfo;
        samplerInfo.mipmapMode = SamplerMipmapMode::SamplerMipmapModeLinear;
        samplerInfo.minLod = 0;
        samplerInfo.maxLod = 15;
        samplerInfo.mipLodBias = 0;

        info.PushStaticSampler(samplerInfo);
        info.paramSlotNum = slots.size();
        info.paramSlots = slots.data();
        rootSignature.reset(cgd->CreateRootSignature(info));
        cbvArgument.reset(
            rootSignature->CreateArgument());

        // Compute Pass RootSig
        RootSignatureCreateInfo compInfo = {};
        std::vector<SignatureSlot> compSlots(2);
        slots[0].type = SignatureSlotType::StorageImageSlot;
        slots[0].stageFlags = ShaderStageFlags::ComputeStage;
		slots[1].type = SignatureSlotType::StorageImageSlot;
		slots[1].stageFlags = ShaderStageFlags::ComputeStage;
        compInfo.paramSlotNum = slots.size();
        compInfo.paramSlots = slots.data();
        compRootSignature.reset(cgd->CreateRootSignature(compInfo));
		compArgument.reset(compRootSignature->CreateArgument());
        
        ComputePipelineCreateInfo compPInfo = {};
        compPInfo.rootSignature = compRootSignature.get();
        compPInfo.shaderStage = csStage;
        compPipeline.reset(cgd->CreateComputePipeline(compPInfo));
    }

    void initVulkan()
    {
        // Create Devices
        Sakura::Graphics::CGDInfo cgd_info;
        cgd = std::make_unique<Sakura::Graphics::Vk::CGD_Vk>();
        cgd_info.enableDebugLayer = true;
        cgd_info.extentionNames = VkSDL_GetInstanceExtensions(win,
            cgd_info.enableDebugLayer);
        cgd_info.extentionNames.push_back
            (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    
        cgd->Initialize(cgd_info);
        SDL_Vulkan_CreateSurface(win,
            ((Sakura::Graphics::Vk::CGD_Vk*)cgd.get())->GetVkInstance(),
            &surface);
        cgd->InitializeDevice(&surface);
        fence.reset(cgd->AllocFence());
        
        createShader();
        createRootSignature();
        ResizeWindow(1280, 720);
        createBuffer();
        cgd->WaitIdle();
    }

    void updateUniformBuffer()
    {
        using namespace Sakura::Math;
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), 
        swapChain->GetExtent().width / (float) swapChain->GetExtent().height,
        0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        constantBuffer->UpdateValue([&](void* ptr) -> void
            {
                memcpy(ptr, &ubo, sizeof(ubo));
            });

        static RootArgumentAttachment attachments[3];
        static UniformBufferAttachment ubAttach;
        static TexSamplerAttachment samplerAttach;
        static TexSamplerAttachment texAttach;
        ubAttach.offset = 0;
        ubAttach.range = sizeof(UniformBufferObject);
        ubAttach.buffer = constantBuffer.get();
        attachments[0].info = ubAttach;
        attachments[0].rootArgType = SignatureSlotType::UniformBufferSlot;
        attachments[0].dstBinding = 0;
        samplerAttach.sampler = sampler.get();
        attachments[1].info = samplerAttach;
        attachments[1].rootArgType = SignatureSlotType::SamplerSlot;
        attachments[1].dstBinding = 1;
        texAttach.imageView = textureTargetView.get();
        texAttach.imageLayout = ImageLayout::ShaderReadOnlyOptimal;
        attachments[2].info = texAttach;
        attachments[2].rootArgType = SignatureSlotType::SampledImageSlot;
        attachments[2].dstBinding = 2;
        cbvArgument->UpdateArgument(attachments, 3);
    }

    void mainLoop()
    {
        auto frameCount = swapChain->GetCurrentFrame();
        RenderTarget rt{&swapChain->GetSwapChainImage(frameCount),
            &swapChain->GetChainImageView(frameCount), {0.f, 0.f, 0.f, 0.f}};
        RenderTarget ds{depth.get(), depthView.get(), {1.f, 0.f}};
        RenderTarget rts[2] = {rt, ds};
        RenderTargetSet rtset{(RenderTarget*)rts, 2};

        context.reset(cgd->CreateContext(ECommandType::CommandContext_Graphics));
		imContext.reset(cgd->CreateContext(ECommandType::CommandContext_Graphics));
        
        //cgd->GetGraphicsQueue()->WaitIdle();
        updateUniformBuffer();
        
        context->Begin();
        context->BeginRenderPass(Pipeline.get(), rtset);
        context->BindVertexBuffers(*vertexBuffer.get());
        context->BindIndexBuffers(*indexBuffer.get());
        const auto* arg = cbvArgument.get();
        context->BindRootArguments(PipelineBindPoint::BindPointGraphics,
            &arg, 1);
        context->DrawIndexed(indices.size(), 1);
        context->EndRenderPass();
        context->End();
        cgd->GetGraphicsQueue()->Submit(context.get());
        
        imContext->Begin();
        profiler->BeginFrame(win);
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Begin("Profiling Board:");                          
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        profiler->Record(imContext.get(), rt);
        imContext->End();
        cgd->GetGraphicsQueue()->Submit(imContext.get());
 
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        cgd->Present(swapChain.get());
    }

    void cleanUp()
    {
        cgd->WaitIdle();
        sampler.reset();
        textureView.reset();
        depthView.reset();
        texture.reset();
        textureTarget.reset();
        textureTargetView.reset();
        depth.reset();
        profiler.reset();
        constantBuffer.reset();
        computeshader.reset();
        vertshader.reset();
        fragshader.reset();
        Pipeline.reset();
        compPipeline.reset();
        swapChain.reset();
        rootSignature.reset();
        compRootSignature.reset();
        compArgument.reset();
        pass.reset();
		fence.reset();
        vertexBuffer.reset();
        indexBuffer.reset();
        context.reset();
        imContext.reset();
        vkDestroySurfaceKHR(((Sakura::Graphics::Vk::CGD_Vk*)cgd.get())->GetVkInstance(),
            surface, nullptr);
	    SDL_DestroyWindow(win);
        SDL_Quit();
        cgd->Destroy();
    }

    
    std::unique_ptr<Sakura::Graphics::CGD> cgd;
    std::unique_ptr<Fence> fence;
    std::unique_ptr<Sakura::Graphics::SwapChain> swapChain;
    std::unique_ptr<CommandContext> context, imContext;

    std::unique_ptr<RootArgument> cbvArgument, compArgument;
    std::unique_ptr<RootSignature> rootSignature, compRootSignature;
    
    std::unique_ptr<Sampler> sampler;
    std::unique_ptr<GpuBuffer> constantBuffer, vertexBuffer, indexBuffer;

    std::unique_ptr<GpuTexture> depth, texture, textureTarget;
    std::unique_ptr<ResourceView> depthView, textureView, textureTargetView;

    ShaderStageCreateInfo vsStage, fsStage, csStage;
    std::unique_ptr<Shader> vertshader, fragshader, computeshader;
    std::unique_ptr<GraphicsPipeline> Pipeline;
    std::unique_ptr<ComputePipeline> compPipeline;
    std::unique_ptr<RenderPass> pass;

    VkSurfaceKHR surface;
    SDL_Window* win = nullptr;
    std::unique_ptr<Sakura::Graphics::Im::ImGuiProfiler> profiler;
};