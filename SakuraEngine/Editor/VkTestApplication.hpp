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
 * @LastEditTime: 2020-03-15 15:03:53
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
#include <thread>
using namespace Sakura;
using namespace Sakura::Graphics::Vk;

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
    {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
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
#if defined(CONFINFO_PLATFORM_LINUX) 
#elif defined(CONFINFO_PLATFORM_MACOS)
		Sakura::fs::file vs_f
		("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\HWVert.spv",
			'r');
		Sakura::fs::file fs_f
		("/Users/saeruhikari/Coding/SakuraEngine/SakuraEngine/UnitTests/UnitTestGraphics/frag.spv",
			'r');
#elif defined(CONFINFO_PLATFORM_WIN32)
		Sakura::fs::file vs_f
		("D:\\Coding\\SakuraEngine\\SakuraTestProject\\shaders\\HWVert.spv",
			'r');
		Sakura::fs::file fs_f
		("D:\\Coding\\SakuraEngine\\SakuraEngine\\UnitTests\\UnitTestGraphics\\frag.spv",
			'r');
#endif

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

    void createVnInfo()
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
        cgd->GetGraphicsQueue()->WaitIdle();
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
		AttachmentReference colorAttachmentRef = {};
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
        ResourceCreateInfo bufferInfo;
        bufferInfo.type = ResourceType::Buffer;
        bufferInfo.detail.buffer.usage = BufferUsage::VertexBuffer;
        bufferInfo.size = sizeof(Vertex) * vertices.size();
        vertexBuffer = std::move(cgd->CreateResource(bufferInfo));
        void* data;
        vertexBuffer->Map(&data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        vertexBuffer->Unmap();
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

        createVnInfo();
        createShader();
        ResizeWindow(1280, 720);
        createBuffer();
    }

    CommandContext* drawTriangle(RenderTargetSet& rts)
    {
        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);
        context->Begin(Pipeline.get());
        context->SetRenderTargets(rts);
        context->BindVertexBuffers(*vertexBuffer.get());
        context->Draw(3, 1, 0, 0);
        context->End();
        return context;
    }

    void mainLoop()
    {
        cgd->Present(swapChain.get());

        auto frameCount = swapChain->GetLastFrame();
        RenderTarget rt{&swapChain->GetSwapChainImage(frameCount),
            &swapChain->GetChainImageView(frameCount)};
        RenderTargetSet rts{&rt, 1};

        SYSTEMTIME t;
        GetLocalTime(&t);
        auto mil = t.wSecond * 1000 + t.wMilliseconds;

        auto drawTri = drawTriangle(rts);

        GetLocalTime(&t);
        mil = t.wMilliseconds + t.wSecond * 1000 - mil;
        std::cout << cgd->contextNum() << "time ms: " << mil << std::endl;
         
        cgd->GetGraphicsQueue()->Submit(drawTri);
        cgd->FreeContext(drawTri);

        static uint64 fenceVal = 0;
        cgd->GetGraphicsQueue()->Submit(fence.get(), fenceVal);
		cgd->Wait(fence.get(), fenceVal);
		fenceVal++;
    }

    void cleanUp()
    {
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
        cgd->Destroy();
	    SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void createWindow()
    {
        win = VkSDL_CreateWindow("SakuraEngine Window: CGD Vulkan", 1280, 720);
    }

    ShaderStageCreateInfo vsStage, fsStage;
    VertexInputStateCreateInfo vbInfo;
    std::unique_ptr<GpuResource> vertexBuffer;
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

