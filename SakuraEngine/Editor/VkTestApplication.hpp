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
 * @LastEditTime: 2020-03-17 10:59:20
 */
#include "SakuraEngine/StaticBuilds/GraphicsInterface/GraphicsCommon/CGD.h"
#include "SakuraEngine/StaticBuilds/GraphicsInterface/CGD_Vulkan/CGD_Vulkan.h"
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
#include "SakuraEngine/StaticBuilds/GraphicsInterface/CGD_Vulkan/Flags/FormatVk.h"
#include "SakuraEngine/StaticBuilds/GraphicsInterface/CGD_Vulkan/Flags/GraphicsPipelineStatesVk.h"
#include "SakuraEngine/StaticBuilds/TaskSystem/TaskSystem.h"
#include "SakuraEngine/StaticBuilds/GraphicsInterface/CGD_Vulkan/GraphicsObjects/FenceVk.h"
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
struct ConstantBuffer
{
    Sakura::Math::Matrix4f model;
    Sakura::Math::Matrix4f view;
    Sakura::Math::Matrix4f proj;
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
		// Create PSO
		std::vector<char> vs_bytes(vs_f.size());
		std::vector<char> fs_bytes(fs_f.size());
		vs_f.read(vs_bytes.data(), vs_bytes.size());
		fs_f.read(fs_bytes.data(), fs_bytes.size());
		vertshader = cgd->CreateShader(vs_bytes.data(), vs_bytes.size());
		fragshader = cgd->CreateShader(fs_bytes.data(), fs_bytes.size());
        // shaders
		vsStage.stage = StageFlags::VertexStage;
		vsStage.shader = vertshader.get(); vsStage.entry = "main";
		fsStage.stage = StageFlags::PixelStage;
		fsStage.shader = fragshader.get(); fsStage.entry = "main";
    }

    void createVInInfo()
    {
        // vertex input
        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();
        vbInfo.vertexBindingDescriptions.resize(1);
        vbInfo.vertexAttributeDescriptions.resize(attributeDescriptions.size());
        vbInfo.vertexBindingDescriptions[0] = bindingDescription;
        vbInfo.vertexAttributeDescriptions[0] = attributeDescriptions[0];
        vbInfo.vertexAttributeDescriptions[1] = attributeDescriptions[1];
    }

    void ResizeWindow(uint32 width, uint32 height)
    {
        cgd->WaitIdle();
		GraphicsPipelineCreateInfo info;
        info.vertexInputInfo = vbInfo;
        info.shaderStages.push_back(vsStage);
		info.shaderStages.push_back(fsStage);

        // recreate swapchain
        swapChain.reset();
        swapChain = std::move(cgd->CreateSwapChain(width, height, &surface));
		Viewport vp = {};
		vp.height = height; vp.width = width;
		Rect2D scissor = {};
		scissor.extent = swapChain->GetExtent();
        
		//Create Render Progress
		RenderProgressCreateInfo rpinfo = {};
		AttachmentDescription colorAttachment;
		colorAttachment.format = swapChain->GetPixelFormat();
		AttachmentReference colorAttachmentRef 
            = {0, ImageLayout::ColorAttachment};
		SubprogressDescription subprog = {};
		subprog.colorAttachments.push_back(colorAttachmentRef);
        
		rpinfo.attachments.push_back(colorAttachment);
		rpinfo.subProcs.push_back(subprog);
        prog.reset();
		prog = std::move(cgd->CreateRenderProgress(rpinfo));

		info.viewportStateCreateInfo.vps.push_back(vp);
		info.viewportStateCreateInfo.scissors.push_back(scissor);
        Pipeline.reset();
		Pipeline = std::move(cgd->CreateGraphicsPipeline(info, *prog.get()));
    }
 
    void createBuffer()
    {
        std::unique_ptr<GpuResource> uploadBuffer, uploadBuffer2;
        ResourceCreateInfo bufferInfo;
        bufferInfo.type = ResourceType::Buffer;
        bufferInfo.detail.buffer.usage = 
            BufferUsage::VertexBuffer | BufferUsage::TransferDst;
        bufferInfo.detail.buffer.cpuAccess = CPUAccessFlags::None;
        bufferInfo.size = sizeof(Vertex) * vertices.size();
        vertexBuffer = std::move(cgd->CreateResource(bufferInfo));

        bufferInfo.detail.buffer.usage = BufferUsage::TransferSrc;
        bufferInfo.detail.buffer.cpuAccess = CPUAccessFlags::ReadWrite;
        uploadBuffer = std::move(cgd->CreateResource(bufferInfo));
        
        void* data;
        uploadBuffer->Map(&data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        uploadBuffer->Unmap();

        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Copy);
        context->Begin();
        context->CopyBuffer(*uploadBuffer.get(),
            *vertexBuffer.get(), bufferInfo.size);


        bufferInfo.size = sizeof(uint16_t) * indices.size();
        uploadBuffer2 = std::move(cgd->CreateResource(bufferInfo));

        bufferInfo.detail.buffer.usage = 
            BufferUsage::IndexBuffer | BufferUsage::TransferDst;
        bufferInfo.detail.buffer.cpuAccess = CPUAccessFlags::None;
        bufferInfo.size = sizeof(uint16_t) * indices.size();
        indexBuffer = std::move(cgd->CreateResource(bufferInfo));

        void* data2;
        uploadBuffer2->Map(&data2);
        memcpy(data2, indices.data(), (size_t)bufferInfo.size);
        uploadBuffer2->Unmap();

        context->CopyBuffer(*uploadBuffer2.get(),
            *indexBuffer.get(), bufferInfo.size);
        context->End();
        
        cgd->GetCopyQueue()->Submit(context);
        cgd->FreeContext(context);
        cgd->GetCopyQueue()->WaitIdle();
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
        fence = std::move(cgd->AllocFence());
        
        createVInInfo();
        createShader();
        ResizeWindow(1280, 720);
        createBuffer();
        
        //profiler = std::make_unique<Sakura::Graphics::Im::ImGuiProfiler>(*cgd.get());
        //profiler->ImGuiInitialize(win, swapChain->GetPixelFormat()); 
    }

    CommandContext* drawTriangle(RenderTargetSet& rts)
    {
        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);
        context->Begin();
        context->BeginRenderPass(Pipeline.get(), rts);
        context->BindVertexBuffers(*vertexBuffer.get());
        context->BindIndexBuffers(*indexBuffer.get());
        context->DrawIndexed(indices.size(), 1);
        context->EndRenderPass();
        context->End();
        return context;
    }

    void mainLoop()
    {
        auto frameCount = swapChain->GetLastFrame();
        RenderTarget rt{&swapChain->GetSwapChainImage(frameCount),
            &swapChain->GetChainImageView(frameCount)};
        RenderTargetSet rts{&rt, 1};

        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);
        context->Begin();
        context->BeginRenderPass(Pipeline.get(), rts);
        context->BindVertexBuffers(*vertexBuffer.get());
        context->BindIndexBuffers(*indexBuffer.get());
        context->DrawIndexed(indices.size(), 1);

    
       /* ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame(win);
        ImGui::NewFrame();
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
            ((const CommandContextVk*)context)->commandBuffer);*/
        context->EndRenderPass();
        context->End();

        static uint64 fenceVal = 1;
        //ImGuiIO& io = ImGui::GetIO(); (void)io;
        /*if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }*/

        cgd->GetGraphicsQueue()
            ->Submit(context, fence.get(), fenceVal - 1, fenceVal);
        cgd->GetGraphicsQueue()->WaitIdle();
        cgd->Wait(fence.get(), fenceVal);
		fenceVal += 1;
        cgd->Present(swapChain.get());
    }

    void cleanUp()
    {
        cgd->WaitIdle();
        cgd->DestroyCommandObjects();
        vertshader.reset();
        fragshader.reset();
        Pipeline.reset();
        swapChain.reset();
        vkDestroySurfaceKHR(((Sakura::Graphics::Vk::CGD_Vk*)cgd.get())->GetVkInstance(),
            surface, nullptr);
        prog.reset();
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

    std::unique_ptr<Sakura::Graphics::Im::ImGuiProfiler> profiler;
    std::unique_ptr<GpuResource> vertexBuffer, indexBuffer;
    ShaderStageCreateInfo vsStage, fsStage;
    VertexInputStateCreateInfo vbInfo;
    std::unique_ptr<Sakura::Graphics::CGD> cgd;
    std::unique_ptr<Fence> fence;
    std::unique_ptr<Shader> vertshader;
    std::unique_ptr<Shader> fragshader;
    std::unique_ptr<Sakura::Graphics::SwapChain> swapChain;
    std::unique_ptr<GraphicsPipeline> Pipeline;
    std::unique_ptr<RenderProgress> prog;
    VkSurfaceKHR surface;
    SDL_Window* win = nullptr;
};

