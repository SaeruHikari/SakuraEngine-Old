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
 * @Date: 2020-02-29 20:05:47
 * @LastEditTime: 2020-03-17 12:26:06
 */
#pragma once
extern "C"
{
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_surface.h"
	#include "SDL2/SDL_vulkan.h"
	#undef main
}
#include "SakuraEngine/Core/Core.h"
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "vulkan/vulkan.h"
using namespace Sakura;
std::vector<const char*> VkSDL_GetInstanceExtensions(SDL_Window* win, 
    bool bOpenValidateLayer = false)
{
    uint extentioncount;
    if(SDL_Vulkan_GetInstanceExtensions(win,
        &extentioncount, nullptr) != SDL_TRUE)
    {
        Sakura::log::error("SDL Vulkan: Failed to get extension num!");
    }
    else
        Sakura::log::debug_info("SDL Vulkan: get total " +
            std::to_string(extentioncount) + " extensions.");
    std::vector<const char*> extentionNames = 
    {
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME // Sample additional extension
    };
    size_t additional_extensions = extentionNames.size();
    extentionNames.resize(additional_extensions + extentioncount);
    if(SDL_Vulkan_GetInstanceExtensions(win,
        &extentioncount, 
        extentionNames.data() + additional_extensions) != SDL_TRUE)
    {
        Sakura::log::error("SDL Vulkan: Failed to get extension names!");
    }
    if(bOpenValidateLayer)
        extentionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return extentionNames;
}

SDL_Window* VkSDL_CreateWindow(
    const char* title = "Vk Window",
    int width = 1280, int height = 720,
    int anchorX = SDL_WINDOWPOS_CENTERED, int anchorY = SDL_WINDOWPOS_CENTERED)
{
    auto win = SDL_CreateWindow(
            title,
            anchorX, anchorY,
            width, height, 
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
    if(!win)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        Sakura::log::debug_error<Sakura::flags::DEBUG_GAME_AND_EDITOR>
            ("Failed to create SDL window!");
    }
    return win;
}
