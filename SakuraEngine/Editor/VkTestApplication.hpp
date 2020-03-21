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
 * @LastEditTime: 2020-03-22 01:54:50
 */
#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
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

    Sakura::fs::file vs_srv
    ("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\SRV\\SRVVertex.spv",
        'r');  
    Sakura::fs::file fs_srv
    ("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\SRV\\SRVPixel.spv",
        'r');
    const std::string texPath =
        "D:\\Coding\\SakuraEngine\\SakuraTestProject\\textures\\chalet.jpg";
    const std::string modlePath = 
        "D:\\Coding\\SakuraEngine\\SakuraTestProject\\models\\chalet.obj";
#endif

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
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
    
    bool operator==(const Vertex& other) const 
    {
        return pos == other.pos && 
            color == other.color && texCoord == other.texCoord;
    }
};
namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) 
            ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) 
            ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}


std::vector<Vertex> vertices;
std::vector<uint32_t> indices;

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
        loadModel();
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
    void loadModel()
    {
        tinyobj::attrib_t attrib;
         std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes,
            &materials, &warn, &err, modlePath.c_str())) 
            throw std::runtime_error(warn + err);

        std::unordered_map<Vertex, uint32_t> uniqueVertices;

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex = {};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] 
                        = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    void createShader()
    {
		// Create PSO vs_cbv
		std::vector<char> vs_bytes(vs_srv.size());
		std::vector<char> fs_bytes(fs_srv.size());
		vs_srv.read(vs_bytes.data(), vs_bytes.size());
		fs_srv.read(fs_bytes.data(), fs_bytes.size());
		vertshader = cgd->CreateShader(vs_bytes.data(), vs_bytes.size());
		fragshader = cgd->CreateShader(fs_bytes.data(), fs_bytes.size());
		// shader Stages
		vsStage.stage = ShaderStageFlags::VertexStage;
		vsStage.shader = vertshader.get(); vsStage.entry = "main";
		fsStage.stage = ShaderStageFlags::PixelStage;
		fsStage.shader = fragshader.get(); fsStage.entry = "main";
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
        info.pipelineLayoutInfo.rootSignature = rootSignature.get();
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
        auto pixels = Sakura::Images::LoadFromDisk<std::byte>(
            texPath.c_str(), width, height, channels);
        std::unique_ptr<GpuBuffer> uploadBuffer;
        uploadBuffer.reset(cgd->CreateUploadBuffer(width * height * 4));
        void* data;
        uploadBuffer->Map(&data);
        memcpy(data, pixels, static_cast<size_t>(width * height * 4));
        uploadBuffer->Unmap();
        
        auto mipLevels = Sakura::Graphics::CalculateMipLevels(width, height);
        texture.reset(
            cgd->CreateGpuTexture(Format::R8G8B8A8_SRGB, width, height,
            ImageUsage::TransferDstImage | ImageUsage::SampledImage |
            ImageUsage::TransferSrcImage, mipLevels));
            
        TextureSubresourceRange textureSubresource;
        textureSubresource.mipLevels = mipLevels;
        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);
        context->Begin();
        context->ResourceBarrier(*texture.get(),
            ImageLayout::Unknown, ImageLayout::TransferDstOptimal,
            textureSubresource);
        context->CopyResource(*uploadBuffer.get(), *texture.get(),
            width, height, ImageAspectFlag::ImageAspectColor);
        context->End();
        cgd->GetGraphicsQueue()->Submit(context);
        cgd->FreeContext(context);
        cgd->GetGraphicsQueue()->WaitIdle();


        context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);
        context->Begin();
        context->GenerateMipmaps(*texture.get(), Format::R8G8B8A8_SRGB,
            width, height, mipLevels);
        context->End();
        cgd->GetGraphicsQueue()->Submit(context);
        cgd->FreeContext(context);
        cgd->GetGraphicsQueue()->WaitIdle();

        ResourceViewCreateInfo tvinfo = {};
        tvinfo.viewType = ResourceViewType::ImageView2D;
        tvinfo.format = Format::R8G8B8A8_SRGB;
        tvinfo.view.texture2D.baseMipLevel = 0;
        tvinfo.view.texture2D.mipLevels = mipLevels;
        tvinfo.view.texture2D.baseArrayLayer = 0;
        tvinfo.view.texture2D.layerCount = 1;
        tvinfo.view.texture2D.aspectMask = ImageAspectFlag::ImageAspectColor;
        textureView.reset(
            cgd->ViewIntoResource(*texture.get(), tvinfo));

        SamplerCreateInfo samplerInfo;
        samplerInfo.mipmapMode = SamplerMipmapMode::SamplerMipmapModeLinear;
        samplerInfo.minLod = 0;
        samplerInfo.maxLod = mipLevels;
        samplerInfo.mipLodBias = 0;
        sampler.reset(cgd->CreateSampler(samplerInfo));
    }
    
    void createDepth()
    {
        depth.reset(cgd->CreateGpuTexture(cgd->FindDepthFormat(),
                swapChain->GetExtent().width, swapChain->GetExtent().height,
                ImageUsage::DepthStencilAttachmentImage));

        ResourceViewCreateInfo dsvinfo = {};
        dsvinfo.viewType = ResourceViewType::ImageView2D;
        dsvinfo.format = cgd->FindDepthFormat();
        dsvinfo.view.texture2D.baseMipLevel = 0;
        dsvinfo.view.texture2D.mipLevels = 1;
        dsvinfo.view.texture2D.baseArrayLayer = 0;
        dsvinfo.view.texture2D.layerCount = 1;
        dsvinfo.view.texture2D.aspectMask = ImageAspectFlag::ImageAspectDepth;
        depthView.reset(
            cgd->ViewIntoResource(*depth.get(), dsvinfo));
    }

    void createBuffer()
    {
        BufferCreateInfo bufferInfo = {};
        bufferInfo.usage = 
            BufferUsage::VertexBuffer | BufferUsage::TransferDstBuffer;
        bufferInfo.cpuAccess = CPUAccessFlags::None;
        bufferInfo.size = sizeof(Vertex) * vertices.size();
        vertexBuffer.reset((GpuBuffer*)cgd->CreateResource(bufferInfo));
        
        std::unique_ptr<GpuBuffer> uploadBuffer, uploadBuffer2;
        uploadBuffer.reset(cgd->CreateUploadBuffer(bufferInfo.size));

        void* data;
        uploadBuffer->Map(&data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        uploadBuffer->Unmap();

        bufferInfo.usage = 
            BufferUsage::IndexBuffer | BufferUsage::TransferDstBuffer;
        bufferInfo.cpuAccess = CPUAccessFlags::None;
        bufferInfo.size = sizeof(uint32_t) * indices.size();
        indexBuffer.reset((GpuBuffer*)cgd->CreateResource(bufferInfo));
        uploadBuffer2.reset(cgd->CreateUploadBuffer(bufferInfo.size));

        void* data2;
        uploadBuffer2->Map(&data2);
        memcpy(data2, indices.data(), (size_t)bufferInfo.size);
        uploadBuffer2->Unmap();
        
        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Copy);
        context->Begin();
        context->CopyResource(*uploadBuffer.get(),
            *vertexBuffer.get(), sizeof(Vertex) * vertices.size());
        context->CopyResource(*uploadBuffer2.get(),
            *indexBuffer.get(), sizeof(uint32_t) * indices.size());
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
            constantBuffers[i].reset((GpuBuffer*)cgd->CreateResource(cbufferInfo));
        createTexture();
    }

    void createRootSignature()
    {
        RootSignatureCreateInfo info = {};
        std::vector<SignatureSlot> slots(2);
        slots[0].type = SignatureSlotType::UniformBufferSlot;
        slots[0].stageFlags = ShaderStageFlags::VertexStage;
        slots[1].type = SignatureSlotType::CombinedTextureSamplerSlot;
        slots[1].stageFlags = ShaderStageFlags::PixelStage;
        info.paramSlotNum = slots.size();
        info.paramSlots = slots.data();
        rootSignature.reset(cgd->CreateRootSignature(info));
        for(auto i = 0; i < 2; i++)
            cbvArgument[i].reset(
                rootSignature->CreateArgument(SignatureSlotType::UniformBufferSlot));
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
        for(auto i = 0; i < 2; i++)
            fence[i].reset(cgd->AllocFence());
        
        createShader();
        createRootSignature();
        ResizeWindow(1280, 720);
        createBuffer();
    }

    void updateUniformBuffer(uint32_t frameCount)
    {
        using namespace Sakura::Math;
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time 
            = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * 0.2f * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), 
        swapChain->GetExtent().width / (float) swapChain->GetExtent().height,
        0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        void* data;
        constantBuffers[frameCount]->Map(&data);
            memcpy(data, &ubo, sizeof(ubo));
        constantBuffers[frameCount]->Unmap();

        RootArgumentAttachment attachments[2];
        UniformBufferAttachment ubAttach;
        ubAttach.offset = 0;
        ubAttach.range = sizeof(UniformBufferObject);
        ubAttach.buffer = constantBuffers[frameCount].get();
        attachments[0].info = ubAttach;
        attachments[0].rootArgType = SignatureSlotType::UniformBufferSlot;
        attachments[0].dstBinding = 0;
        TexSamplerAttachment texAttach;
        texAttach.imageView = textureView.get();
        texAttach.sampler = sampler.get();
        texAttach.imageLayout = ImageLayout::ShaderReadOnlyOptimal;
        attachments[1].info = texAttach;
        attachments[1].rootArgType = SignatureSlotType::CombinedTextureSamplerSlot;
        attachments[1].dstBinding = 1;
        cbvArgument[frameCount]->UpdateArgument(attachments, 2);
    }

    void mainLoop()
    {
        cgd->Wait(
            fence[swapChain->GetCurrentFrame()].get(),
            fence[swapChain->GetCurrentFrame()]->GetCompletedValue());
        cgd->Present(swapChain.get());

        auto frameCount = swapChain->GetCurrentFrame();
        RenderTarget rt{&swapChain->GetSwapChainImage(frameCount),
            &swapChain->GetChainImageView(frameCount), {0.f, 0.f, 0.f, 0.f}};
        RenderTarget ds{depth.get(), depthView.get(), {1.f, 0.f}};
        RenderTarget rts[2] = {rt, ds};
        RenderTargetSet rtset{(RenderTarget*)rts, 2};

        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);

        updateUniformBuffer(frameCount);
        context->Begin();
        context->BeginRenderPass(Pipeline.get(), rtset);
        context->BindVertexBuffers(*vertexBuffer.get());
        context->BindIndexBuffers(*indexBuffer.get());
        const auto* arg = cbvArgument[frameCount].get();
        context->BindRootArguments(PipelineBindPoint::BindPointGraphics,
            &arg, 1);
        context->DrawIndexed(indices.size(), 1);
        context->EndRenderPass();
        context->End();
        cgd->GetGraphicsQueue()->Submit(context);
        
		auto imContext =
			cgd->AllocateContext(ECommandType::CommandContext_Graphics);
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
        profiler->Record(imContext, rt);
        imContext->End();
        cgd->GetGraphicsQueue()->Submit(imContext, 
        fence[frameCount].get(),
        fence[frameCount]->GetCompletedValue(),
        fence[frameCount]->GetCompletedValue() + 1);
 
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        
        cgd->FreeContext(context);
        cgd->FreeContext(imContext);
    }

    void cleanUp()
    {
        cgd->WaitIdle();
        sampler.reset();
        textureView.reset();
        depthView.reset();
        texture.reset();
        depth.reset();
        profiler.reset();
        for(auto i = 0; i < constantBuffers.size(); i++)
            constantBuffers[i].reset();
        vertshader.reset();
        fragshader.reset();
        Pipeline.reset();
        swapChain.reset();
        rootSignature.reset();
        vkDestroySurfaceKHR(((Sakura::Graphics::Vk::CGD_Vk*)cgd.get())->GetVkInstance(),
            surface, nullptr);
        pass.reset();
        for(auto i = 0; i < 2; i++)
            fence[i].reset();
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

    std::unique_ptr<RootArgument> cbvArgument[2];
    std::unique_ptr<RootSignature> rootSignature;
    // Resources
    std::unique_ptr<Sampler> sampler;
    std::vector<std::unique_ptr<GpuBuffer>> constantBuffers;
    std::unique_ptr<GpuBuffer> vertexBuffer, indexBuffer;

    std::unique_ptr<GpuTexture> texture;
    std::unique_ptr<ResourceView> textureView;

    std::unique_ptr<GpuTexture> depth;
    std::unique_ptr<ResourceView> depthView;

    std::unique_ptr<Sakura::Graphics::Im::ImGuiProfiler> profiler;
    ShaderStageCreateInfo vsStage, fsStage;
    std::unique_ptr<Sakura::Graphics::CGD> cgd;
    std::unique_ptr<Fence> fence[2];
    std::unique_ptr<Shader> vertshader;
    std::unique_ptr<Shader> fragshader;
    std::unique_ptr<Sakura::Graphics::SwapChain> swapChain;
    std::unique_ptr<GraphicsPipeline> Pipeline;
    std::unique_ptr<RenderPass> pass;
    VkSurfaceKHR surface;
    SDL_Window* win = nullptr;
};

