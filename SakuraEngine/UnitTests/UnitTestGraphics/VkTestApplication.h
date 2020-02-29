/*
 * @This File is Part of Sakura by SaeruHikari: 
 *    This is the test application of vulkan learning.  
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-29 11:46:00
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-29 21:46:38
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
        Sakura::Graphics::CGD_Info cgd_info;
        cgd_info.enableDebugLayer = true;
        cgd_info.extentionNames = VkSDL_GetInstanceExtensions(win,
            cgd_info.enableDebugLayer);
        Sakura::Graphics::CGD::Initialize
            (Sakura::Graphics::CGD::TargetGraphicsInterface::CGD_TARGET_VULKAN,
            cgd_info);
    }

    void mainLoop()
    {
        Sakura::Graphics::CGD::GetCGD()->Render();
    }

    void cleanUp()
    {
        Sakura::Graphics::CGD::GetCGD()->Destroy();
	    SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void createWindow()
    {
        win = VkSDL_CreateWindow();
    }

    SDL_Window* win = nullptr;
};

