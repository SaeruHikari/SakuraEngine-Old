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
 * @LastEditTime: 2020-03-10 18:14:36
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
using namespace Sakura;

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
                if (evt.type == SDL_QUIT)
                    run = 0;
            }
            mainLoop();
        }
        cleanUp();
    }

private:
    void initVulkan()
    {
        // Create Devices
        Sakura::Graphics::CGDInfo cgd_info;
        cgd = new Sakura::Graphics::Vk::CGD_Vk();
        cgd_info.enableDebugLayer = true;
        cgd_info.extentionNames = VkSDL_GetInstanceExtensions(win,
            cgd_info.enableDebugLayer);
        cgd->Initialize(cgd_info);
        SDL_Vulkan_CreateSurface(win,
            ((Sakura::Graphics::Vk::CGD_Vk*)cgd)->GetVkInstance(), &surface);
        cgd->InitQueueSet(&surface);
        swapChain = std::move(cgd->CreateSwapChain(width, height, &surface));

        //Create Render Progress
        RenderProgressCreateInfo rpinfo = {};
        AttachmentDescription colorAttachment;
        colorAttachment.format = swapChain->GetPixelFormat();
        AttachmentReference colorAttachmentRef = {};
        SubprogressDescription subprog = {};
        subprog.colorAttachments.push_back(colorAttachmentRef);
        rpinfo.attachments.push_back(colorAttachment);
        rpinfo.subProcs.push_back(subprog);
        prog = std::move(cgd->CreateRenderProgress(rpinfo));

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
        GraphicsPipelineCreateInfo info;
        ShaderStageCreateInfo vsStage, fsStage;
        vsStage.stage = StageFlags::VertexStage;
        vsStage.shader = vertshader.get();vsStage.entry = "main";
        info.shaderStages.push_back(vsStage);
        fsStage.stage = StageFlags::PixelStage;
        fsStage.shader = fragshader.get();fsStage.entry = "main";
        info.shaderStages.push_back(fsStage);
        Viewport vp = {};
        vp.height = 720; vp.width = 1280;
        Rect2D scissor = {};
        scissor.extent = swapChain->GetExtent();
        info.viewportStateCreateInfo.vps.push_back(vp);
        info.viewportStateCreateInfo.scissors.push_back(scissor);
        Pipeline = std::move(cgd->CreateGraphicsPipeline(info, *prog.get()));
        
        for(auto i = 0; i < 2; i ++)
        {
            RenderTarget rt{&swapChain->GetSwapChainImage(i),
                &swapChain->GetChainImageView(i)};
            RenderTargetSet rts{&rt, 1};
            cmdContexts[i] = 
                cgd->AllocateContext(ECommandType::CommandContext_Graphics);
            cmdContexts[i]->Begin(Pipeline.get());
            cmdContexts[i]->SetRenderTargets(rts);
            cmdContexts[i]->Draw(3, 1, 0, 0);
            cmdContexts[i]->End();
        }
    }

    void mainLoop()
    {
        static uint currentFrame = 0;
        cgd->GetGraphicsQueue()->Submit(cmdContexts[currentFrame]);
        cgd->Present(swapChain.get());
        currentFrame = (currentFrame + 1) % 2;
    }

    void cleanUp()
    {
        Pipeline.reset();
        vertshader.reset();
        fragshader.reset();
        swapChain.reset();
        vkDestroySurfaceKHR(((Sakura::Graphics::Vk::CGD_Vk*)cgd)->GetVkInstance(),
            surface, nullptr);
        cgd->Destroy();
	    SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void createWindow()
    {
        win = VkSDL_CreateWindow("Vk Window", width, height);
    }
    
    const int width = 1280;
    const int height = 720;
    Sakura::Graphics::CGD* cgd;
    std::unique_ptr<Shader> vertshader;
    std::unique_ptr<Shader> fragshader;
    CommandContext* cmdContexts[2];
    std::unique_ptr<Sakura::Graphics::SwapChain> swapChain;
    std::unique_ptr<GraphicsPipeline> Pipeline;
    std::unique_ptr<RenderProgress> prog;
    VkSurfaceKHR surface;
    SDL_Window* win = nullptr;
};

