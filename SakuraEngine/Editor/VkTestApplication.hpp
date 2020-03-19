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
 * @LastEditTime: 2020-03-19 22:26:25
 */
#pragma once
#define GLM_FORCE_RADIANS
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
    Sakura::fs::file vs_f
    ("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\VertexBuffer\\HWVert.spv",
        'r');
    Sakura::fs::file fs_f
    ("D:\\Coding\\SakuraEngine\\SakuraEngine\\UnitTests\\UnitTestGraphics\\frag.spv",
        'r');
    Sakura::fs::file vs_cbv
    ("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\CBV\\CBVVert.spv",
        'r');
    const std::string texPath =
        "D:\\Coding\\SakuraEngine\\SakuraTestProject\\textures\\640x640.jpg";
#endif

struct Vertex
{
    Sakura::Math::Vector2f pos;
    Sakura::Math::Vector3f color;
    static VertexInputBindingDescription getBindingDescription() {
        VertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VertexInputRate::VertexInputRateVertex;

        return bindingDescription;
    }

    static std::array<VertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = Format::R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = Format::R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};
const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
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
        createWindow();
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
		std::vector<char> vs_bytes(vs_cbv.size());
		std::vector<char> fs_bytes(fs_f.size());
		vs_cbv.read(vs_bytes.data(), vs_bytes.size());
		fs_f.read(fs_bytes.data(), fs_bytes.size());
		vertshader = cgd->CreateShader(vs_bytes.data(), vs_bytes.size());
		fragshader = cgd->CreateShader(fs_bytes.data(), fs_bytes.size());
        // shaders
		vsStage.stage = ShaderStageFlags::VertexStage;
		vsStage.shader = vertshader.get(); vsStage.entry = "main";
		fsStage.stage = ShaderStageFlags::PixelStage;
		fsStage.shader = fragshader.get(); fsStage.entry = "main";
    }

    void ResizeWindow(uint32 width, uint32 height)
    {
        cgd->WaitIdle();
		GraphicsPipelineCreateInfo info;
        std::vector<ShaderStageCreateInfo> infos{vsStage, fsStage};
		info.shaderStages = infos.data();
        info.shaderStageCount = infos.size();
        info.pipelineLayoutInfo.setLayouts = rootSignature.get();
        // recreate swapchain
        swapChain.reset(cgd->CreateSwapChain(width, height, &surface));
		Viewport vp = {};
		vp.height = height; vp.width = width;
		Rect2D scissor = {};
		scissor.extent = swapChain->GetExtent();
        
		//Create Render Pass
		RenderPassCreateInfo rpinfo = {};
		AttachmentDescription colorAttachment;
		colorAttachment.format = swapChain->GetPixelFormat();
		AttachmentReference colorAttachmentRef 
            = {0, ImageLayout::ColorAttachment};
		SubpassDescription subpass = {};
		subpass.colorAttachments.push_back(colorAttachmentRef);
        
		rpinfo.attachments.push_back(colorAttachment);
		rpinfo.subProcs.push_back(subpass);
		pass.reset(cgd->CreateRenderPass(rpinfo));
        
         // vertex input
        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();
        std::vector<VertexInputBindingDescription>
            vertexBindingDescriptions{bindingDescription};
        std::vector<VertexInputAttributeDescription> 
            vertexAttributeDescriptions{attributeDescriptions[0], attributeDescriptions[1]};
        VertexInputStateCreateInfo vbInfo = {};
        vbInfo.vertexAttributeDescriptions = vertexAttributeDescriptions.data();
        vbInfo.vertexAttributeDescriptionCount = (uint32)vertexAttributeDescriptions.size();
        vbInfo.vertexBindingDescriptions = vertexBindingDescriptions.data();
        vbInfo.vertexBindingDescriptionCount = (uint32)vertexBindingDescriptions.size();
        info.vertexInputInfo = vbInfo;
		info.viewportStateCreateInfo.vps = &vp;
		info.viewportStateCreateInfo.vpCount = 1;
		info.viewportStateCreateInfo.scissors = &scissor;
		info.viewportStateCreateInfo.scissorCount = 1;
		Pipeline.reset(cgd->CreateGraphicsPipeline(info, *pass.get()));
    }
    void createTexture()
    {
        int width, height, channels;
        auto pixels = Sakura::Images::LoadFromDisk<std::byte>(
            texPath.c_str(), width, height, channels);
        std::unique_ptr<GpuBuffer> uploadBuffer;
        uploadBuffer.reset(cgd->CreateUploadBuffer(width * height * 4));
        void* data;
        uploadBuffer->Map(&data);
        memcpy(data, pixels, static_cast<size_t>(width * height * 4));
        uploadBuffer->Unmap();
        
        TextureCreateInfo imgInfo = {};
        imgInfo.width = width;
        imgInfo.height = height;
        imgInfo.format = Format::R8G8B8A8_SRGB;
        imgInfo.arrayLayers = 1;
        imgInfo.depth = 1;
        imgInfo.mipLevels = 1;
        imgInfo.usage = 
            ImageUsage::TransferDstImage | ImageUsage::SampledImage;
        texture.reset(
            cgd->CreateResource(imgInfo));
        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);
        context->Begin();
        context->ResourceBarrier(*texture.get(),
            ImageLayout::Unknown, ImageLayout::TransferDstOptimal);
        context->CopyResource(*uploadBuffer.get(), *texture.get(),
            width, height, ImageAspectFlag::ImageAspectColor);
        context->ResourceBarrier(*texture.get(),
            ImageLayout::TransferDstOptimal, ImageLayout::ShaderReadOnlyOptimal);
        context->End();
        cgd->GetGraphicsQueue()->Submit(context);
        cgd->FreeContext(context);
        cgd->GetGraphicsQueue()->WaitIdle();
        ResourceViewCreateInfo tvinfo = {};
        tvinfo.viewType = ResourceViewType::ImageView2D;
        tvinfo.format = Format::R8G8B8A8_SRGB;
        tvinfo.view.texture2D.baseMipLevel = 0;
        tvinfo.view.texture2D.mipLevels = 1;
        tvinfo.view.texture2D.baseArrayLayer = 0;
        tvinfo.view.texture2D.layerCount = 1;
        tvinfo.view.texture2D.aspectMask = ImageAspectFlag::ImageAspectColor;
        textureView.reset(
            cgd->ViewIntoResource(*texture.get(), tvinfo));
    }
    
    void createBuffer()
    {
        BufferCreateInfo bufferInfo = {};
        bufferInfo.usage = 
            BufferUsage::VertexBuffer | BufferUsage::TransferDstBuffer;
        bufferInfo.cpuAccess = CPUAccessFlags::None;
        bufferInfo.size = sizeof(Vertex) * vertices.size();
        vertexBuffer.reset((GpuBuffer*)cgd->CreateResource(bufferInfo));
        
        std::unique_ptr<GpuBuffer> uploadBuffer;
        uploadBuffer.reset(cgd->CreateUploadBuffer(bufferInfo.size));

        void* data;
        uploadBuffer->Map(&data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        uploadBuffer->Unmap();

        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Copy);
        context->Begin();
        context->CopyResource(*uploadBuffer.get(),
            *vertexBuffer.get(), bufferInfo.size);

        std::unique_ptr<GpuBuffer> uploadBuffer2;
        bufferInfo.size = sizeof(uint16_t) * indices.size();
        uploadBuffer2.reset(cgd->CreateUploadBuffer(bufferInfo.size));

        bufferInfo.usage = 
            BufferUsage::IndexBuffer | BufferUsage::TransferDstBuffer;
        bufferInfo.cpuAccess = CPUAccessFlags::None;
        bufferInfo.size = sizeof(uint16_t) * indices.size();
        indexBuffer.reset((GpuBuffer*)cgd->CreateResource(bufferInfo));

        void* data2;
        uploadBuffer2->Map(&data2);
        memcpy(data2, indices.data(), (size_t)bufferInfo.size);
        uploadBuffer2->Unmap();

        context->CopyResource(*uploadBuffer2.get(),
            *indexBuffer.get(), bufferInfo.size);
        context->End();
        
        cgd->GetCopyQueue()->Submit(context);
        cgd->FreeContext(context);
        cgd->GetCopyQueue()->WaitIdle();

        constantBuffers.resize(swapChain->GetSwapChainCount());
        BufferCreateInfo cbufferInfo = {};
        cbufferInfo.usage = 
            BufferUsage::ConstantBuffer;
        cbufferInfo.cpuAccess 
            = CPUAccessFlags::ReadWrite;
        cbufferInfo.size = sizeof(UniformBufferObject);
        for(auto i = 0u; i < constantBuffers.size(); i++)
        {
            constantBuffers[i].reset((GpuBuffer*)cgd->CreateResource(cbufferInfo));
        }
        createTexture();
    }

    void createRootSignature()
    {
        RootSignatureCreateInfo info = {};
        std::vector<SignatureSlot> slots(1);
        slots[0].type = SignatureSlotType::UniformBufferSlot;
        slots[0].stageFlags = ShaderStageFlags::VertexStage;
        //slots[1].type = SignatureSlotType::SamplerSlot;
        //slots[1].stageFlags = ShaderStageFlags::PixelStage;
        info.paramSlotNum = slots.size();
        info.paramSlots = slots.data();
        rootSignature.reset(cgd->CreateRootSignature(info));
        cbvArgument.reset(
            rootSignature->CreateArgument(SignatureSlotType::UniformBufferSlot));
    }

    void createSampler()
    {
        SamplerCreateInfo samplerInfo;
        sampler.reset(cgd->CreateSampler(samplerInfo));
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
        cgd->InitQueueSet(&surface);
        fence.reset(cgd->AllocFence());
        
        createShader();
        createRootSignature();
        ResizeWindow(1280, 720);
        createBuffer();
        createSampler();
        cgd->WaitIdle();
        profiler = std::make_unique<Sakura::Graphics::Im::ImGuiProfiler>(*cgd.get());
        profiler->ImGuiInitialize(win, swapChain->GetPixelFormat()); 
    }

    void updateUniformBuffer(uint32_t frameCount)
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

        void* data;
        constantBuffers[frameCount]->Map(&data);
            memcpy(data, &ubo, sizeof(ubo));
        constantBuffers[frameCount]->Unmap();

        RootArgumentAttachment attachment = {};
        attachment.info.uniformBuffer.offset = 0;
        attachment.info.uniformBuffer.range = sizeof(UniformBufferObject);
        attachment.info.uniformBuffer.buffer = constantBuffers[frameCount].get();
        cbvArgument->UpdateArgument(attachment);
    }

    void mainLoop()
    {
        auto frameCount = swapChain->GetLastFrame();
        RenderTarget rt{&swapChain->GetSwapChainImage(frameCount),
            &swapChain->GetChainImageView(frameCount)};
        RenderTargetSet rts{&rt, 1};
        updateUniformBuffer(frameCount);
        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);
        context->Begin();
        context->BeginRenderPass(Pipeline.get(), rts);
        context->BindVertexBuffers(*vertexBuffer.get());
        context->BindIndexBuffers(*indexBuffer.get());
        const auto* arg = cbvArgument.get();
        context->BindRootArguments(PipelineBindPoint::BindPointGraphics,
            &arg, 1);
        context->DrawIndexed(indices.size(), 1);
    
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame(win);
        ImGui::NewFrame();
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
            ((const CommandContextVk*)context)->commandBuffer);
        context->EndRenderPass();
        context->End();

        static uint64 fenceVal = 1;
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        cgd->GetGraphicsQueue()->Submit(context);
        cgd->GetGraphicsQueue()->Submit(fence.get(), fenceVal);
        cgd->GetGraphicsQueue()->WaitIdle();
        cgd->Wait(fence.get(), fenceVal);
		fenceVal += 1;
        cgd->Present(swapChain.get());

        cgd->FreeContext(context);
    }

    void cleanUp()
    {
        cgd->WaitIdle();
        sampler.reset();
        textureView.reset();
        profiler.reset();
        for(auto i = 0; i < constantBuffers.size(); i++)
            constantBuffers[i].reset();
        texture.reset();
        vertshader.reset();
        fragshader.reset();
        Pipeline.reset();
        swapChain.reset();
        rootSignature.reset();
        vkDestroySurfaceKHR(((Sakura::Graphics::Vk::CGD_Vk*)cgd.get())->GetVkInstance(),
            surface, nullptr);
        pass.reset();
		fence.reset();
        vertexBuffer.reset();
        indexBuffer.reset();
        cgd->Destroy();
	    SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void createWindow()
    {
        win = VkSDL_CreateWindow("SakuraEngine Window: CGD Vulkan", 1280, 720);
    }

    std::unique_ptr<RootArgument> cbvArgument;
    std::unique_ptr<RootSignature> rootSignature;
    // Resources
    std::unique_ptr<Sampler> sampler;
    std::vector<std::unique_ptr<GpuBuffer>> constantBuffers;
    std::unique_ptr<GpuBuffer> vertexBuffer, indexBuffer;
    std::unique_ptr<GpuTexture> texture;

    std::unique_ptr<ResourceView> textureView;
    std::unique_ptr<Sakura::Graphics::Im::ImGuiProfiler> profiler;
    ShaderStageCreateInfo vsStage, fsStage;
    std::unique_ptr<Sakura::Graphics::CGD> cgd;
    std::unique_ptr<Fence> fence;
    std::unique_ptr<Shader> vertshader;
    std::unique_ptr<Shader> fragshader;
    std::unique_ptr<Sakura::Graphics::SwapChain> swapChain;
    std::unique_ptr<GraphicsPipeline> Pipeline;
    std::unique_ptr<RenderPass> pass;
    VkSurfaceKHR surface;
    SDL_Window* win = nullptr;
};

