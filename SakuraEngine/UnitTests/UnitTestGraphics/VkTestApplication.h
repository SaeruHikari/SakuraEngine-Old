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
 * @LastEditTime: 2020-03-09 01:35:12
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
#include "SakuraEngine/StaticBuilds/GraphicsInterface/CGD_Vulkan/Flags/PixelFormatVk.h"
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
            run = 0;
            mainLoop();
        }
        cleanUp();
    }

private:
    void initVulkan()
    {
        Sakura::Graphics::CGDInfo cgd_info;
        cgd = new Sakura::Graphics::Vk::CGD_Vk();
        cgd_info.enableDebugLayer = true;
        cgd_info.extentionNames = VkSDL_GetInstanceExtensions(win,
            cgd_info.enableDebugLayer);
        cgd->Initialize(cgd_info);
        SDL_Vulkan_CreateSurface(win,
            ((Sakura::Graphics::Vk::CGD_Vk*)cgd)->GetVkInstance(), &surface);
        cgd->InitQueueSet(&surface);
        swapChain = std::move(
            cgd->CreateSwapChain(width, height, &surface));
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
        GraphicsPipelineCreateInfo info = {};
        ShaderStageCreateInfo vsStage, fsStage;
        vsStage.stage = StageFlags::VertexStage;
        vsStage.shader = vertshader.get();
        vsStage.entry = "main";
        fsStage.stage = StageFlags::PixelStage;
        fsStage.shader = fragshader.get();
        fsStage.entry = "main";
        info.stages.push_back(vsStage);
        info.stages.push_back(fsStage);
        Viewport vp = {};
        vp.height = 720; vp.width = 1280;
        Rect2D scissor = {};
        scissor.extent = swapChain->GetExtent();
        info.viewportStateCreateInfo.vps.push_back(vp);
        info.viewportStateCreateInfo.scissors.push_back(scissor);
        auto pso = cgd->CreateGraphicsPipeline(info);

        RenderPassCreateInfo rpinfo = {};
        AttachmentDescription colorAttachment;
        colorAttachment.format = swapChain->GetPixelFormat();

        AttachmentReference colorAttachmentRef = {};
        SubpassDescription subpass = {};
        subpass.colorAttachments.push_back(colorAttachmentRef);

        rpinfo.attachments.push_back(colorAttachment);
        rpinfo.subPasses.push_back(subpass);

        auto pass = cgd->CreateRenderPass(rpinfo);
    }

    void mainLoop()
    {
        cgd->Render();
    }

    void cleanUp()
    {
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
    std::unique_ptr<Sakura::Graphics::SwapChain> swapChain;
    VkSurfaceKHR surface;
    SDL_Window* win = nullptr;
};

