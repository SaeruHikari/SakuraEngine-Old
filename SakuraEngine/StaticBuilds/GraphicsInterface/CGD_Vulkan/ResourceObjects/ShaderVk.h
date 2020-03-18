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
 * @Date: 2020-03-06 20:43:34
 * @LastEditTime: 2020-03-18 08:12:27
 */
#pragma once
#include "../../GraphicsCommon/ResourceObjects/Shader.h"
#include "vulkan/vulkan.h"

using namespace Sakura::Graphics;

namespace Sakura::Graphics::Vk
{
    inline static bool CheckAvailable(ShaderType type){return true;}
    inline static bool CheckAvailable(ShaderSourceType type){return true;}
    inline static bool CheckAvailable(ShaderCompileFlags flags){return true;}
    inline static bool CheckAvailable(ShaderStageFlags flags){return true;}

	sinline static VkShaderStageFlagBits Transfer(ShaderStageFlags info)
	{
        return VkShaderStageFlagBits(info);
	}

	sinline static ShaderStageFlags Transfer(VkShaderStageFlagBits info)
	{
		return ShaderStageFlags(info);
	}

    struct ShaderVk final : public Shader
    {
        ShaderVk(const VkDevice& _device)
            : device(_device){}
        virtual ~ShaderVk() override final;
        VkShaderModule shaderModule;
        const VkDevice& device;
    };
}