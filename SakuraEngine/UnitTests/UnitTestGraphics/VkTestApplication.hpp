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
 * @LastEditTime: 2020-03-12 21:24:11
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
		("/Users/saeruhikari/Coding/SakuraEngine/SakuraEngine/UnitTests/UnitTestGraphics/vert.spv",
			'r');
		Sakura::fs::file fs_f
		("/Users/saeruhikari/Coding/SakuraEngine/SakuraEngine/UnitTests/UnitTestGraphics/frag.spv",
			'r');
#elif defined(CONFINFO_PLATFORM_WIN32)
		Sakura::fs::file vs_f
		("D:\\Coding\\SakuraEngine\\SakuraEngine\\UnitTests\\UnitTestGraphics\\vert.spv",
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
    }

    void ResizeWindow(uint32 width, uint32 height)
    {
        cgd->GetGraphicsQueue()->WaitIdle();
		GraphicsPipelineCreateInfo info;
		ShaderStageCreateInfo vsStage, fsStage;
		vsStage.stage = StageFlags::VertexStage;
		vsStage.shader = vertshader.get(); vsStage.entry = "main";
		info.shaderStages.push_back(vsStage);
		fsStage.stage = StageFlags::PixelStage;
		fsStage.shader = fragshader.get(); fsStage.entry = "main";
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

    void createPSO()
    {
        createShader();
		ResizeWindow(1280, 720);
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
        cgd_info.extentionNames.push_back
            ("VK_KHR_get_physical_device_properties2");
    
        cgd->Initialize(cgd_info);
        SDL_Vulkan_CreateSurface(win,
            ((Sakura::Graphics::Vk::CGD_Vk*)cgd.get())->GetVkInstance(),
            &surface);
        cgd->InitQueueSet(&surface);
        fence = std::move(cgd->AllocFence());

        createPSO();
    }

    CommandContext* drawTriangle(RenderTargetSet& rts)
    {
        auto context = 
            cgd->AllocateContext(ECommandType::CommandContext_Graphics);
        context->Begin(Pipeline.get());
        context->SetRenderTargets(rts);
        context->Draw(3, 1, 0, 0);
        context->End();
        return context;
    }
    
    void SubmitAndFree(CommandContext* toSub, CommandQueue* queue)
    {
        queue->Submit(toSub);
        cgd->FreeContext(toSub);
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
         
        SubmitAndFree(drawTri, cgd->GetGraphicsQueue()); 
        static uint64 fenceVal = 0;
		cgd->Wait(fence.get(), fenceVal);
		fenceVal++;
        cgd->GetGraphicsQueue()->Submit(fence.get(), fenceVal);
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
        cgd->Destroy();
	    SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void createWindow()
    {
        win = VkSDL_CreateWindow("SakuraEngine Window: CGD Vulkan", 1280, 720);
        
    }

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

