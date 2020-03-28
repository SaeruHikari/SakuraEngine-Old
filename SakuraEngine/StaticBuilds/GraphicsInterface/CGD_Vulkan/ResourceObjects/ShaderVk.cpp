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
 * @Date: 2020-03-06 20:51:22
 * @LastEditTime: 2020-03-06 23:01:17
 */
#include "ShaderVk.h"
#include "../CGD_Vulkan.h"

using namespace Sakura::Graphics;
using namespace Sakura::Graphics::Vk;

ShaderVk::~ShaderVk()
{
    vkDestroyShaderModule(device, shaderModule, nullptr);
}

std::unique_ptr<Shader> CGDVk::CreateShader(
    const char* data, std::size_t dataSize)
{
    auto shader = std::make_unique<ShaderVk>(entityVk.device);
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = dataSize;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(data);
    if (vkCreateShaderModule(entityVk.device, &createInfo,
        nullptr, &shader->shaderModule) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create shader module!");
    }
    return std::move(shader);
}

const char* CGDVk::CompileShader(const char* src, std::size_t srcSize)
{
    return nullptr;
}

