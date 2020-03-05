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
 * @LastEditTime: 2020-03-05 20:26:58
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
#include "SakuraEngine/StaticBuilds/GraphicsInterface/CGD_Vulkan/Vulkan_FormatTransfer.h"
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
        using Sakura::Graphics::Vk::CGD_Vk;
        Sakura::Graphics::CGDInfo cgd_info;
        cgd_info.enableDebugLayer = true;
        cgd_info.extentionNames = VkSDL_GetInstanceExtensions(win,
            cgd_info.enableDebugLayer);
        using CGD = CGD_Vk;
        CGD::Initialize(cgd_info, entityVk);
        SDL_Vulkan_CreateSurface(win, entityVk.GetVkInstance(), &surface);
        CGD::InitQueueSet(&surface, entityVk);
        CGD::CreateSwapChain(width, height, entityVk, &surface);
    }

    void mainLoop()
    {
        using Sakura::Graphics::Vk::CGD_Vk;
        using CGD = CGD_Vk;
        CGD::Render(entityVk);
    }

    void cleanUp()
    {
        using Sakura::Graphics::Vk::CGD_Vk;
        using CGD = CGD_Vk;
        vkDestroySurfaceKHR(entityVk.GetVkInstance(), surface, nullptr);
        CGD::Destroy(entityVk);
	    SDL_DestroyWindow(win);
        VkFormat fmt = Sakura::Graphics::Vk::Transfer(
            Sakura::Graphics::PixelFormat::R32G32B32A32_UINT);
        VkFormat fmt2 = Sakura::Graphics::Vk::Transfer(
            Sakura::Graphics::PixelFormat::S_R32G32B32A32_UINT);
        SDL_Quit();
    }

    void createWindow()
    {
        win = VkSDL_CreateWindow("Vk Window", width, height);
    }
    
    const int width = 1280;
    const int height = 720;
    Sakura::Graphics::Vk::CGDEntityVk entityVk;
    VkSurfaceKHR surface;
    SDL_Window* win = nullptr;
};

