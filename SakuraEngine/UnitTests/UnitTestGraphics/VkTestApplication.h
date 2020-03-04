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
 * @LastEditTime: 2020-03-04 18:56:25
 */
#include "SakuraEngine/Modules/GraphicsInterface/GraphicsCommon/CGD.h"
#include "SakuraEngine/Modules/GraphicsInterface/CGD_Vulkan/CGD_Vulkan.h"
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
        Sakura::Graphics::CGD_Info cgd_info;
        cgd_info.enableDebugLayer = true;
        cgd_info.extentionNames = VkSDL_GetInstanceExtensions(win,
            cgd_info.enableDebugLayer);
        Sakura::Graphics::CGD::Initialize
            (Sakura::Graphics::CGD::TargetGraphicsInterface::CGD_TARGET_VULKAN,
            cgd_info);
        vkCGD = (CGD_Vk*)Sakura::Graphics::CGD::GetCGD();
        SDL_Vulkan_CreateSurface(win, vkCGD->GetVkInstance(), &surface);
        vkCGD->InitQueueSet(&surface);
    }

    void mainLoop()
    {
        Sakura::Graphics::CGD::GetCGD()->Render();
    }

    void cleanUp()
    {
        vkDestroySurfaceKHR(vkCGD->GetVkInstance(), surface, nullptr);
        Sakura::Graphics::CGD::GetCGD()->Destroy();
	    SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void createWindow()
    {
        win = VkSDL_CreateWindow();
    }
    
    Sakura::Graphics::Vk::CGD_Vk* vkCGD = nullptr;
    VkSurfaceKHR surface;
    SDL_Window* win = nullptr;
};

