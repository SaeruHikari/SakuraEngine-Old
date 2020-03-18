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
 * @Date: 2020-03-07 18:07:10
 * @LastEditTime: 2020-03-18 09:04:48
 */
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
 * @Date: 2020-03-06 10:37:15
 * @LastEditTime: 2020-03-06 15:53:11
 */
#pragma once
#include "../../GraphicsCommon/ResourceObjects/ResourceViews.h"
#include "SakuraEngine/Core/EngineUtils/log.h"
#include "vulkan/vulkan.h"

#define VIEW_TYPE_MAPPER() \
VK_VIEW_TYPE_MAPPING(ImageView1D, VK_IMAGE_VIEW_TYPE_1D); \
VK_VIEW_TYPE_MAPPING(ImageView2D, VK_IMAGE_VIEW_TYPE_2D); \
VK_VIEW_TYPE_MAPPING(ImageView3D, VK_IMAGE_VIEW_TYPE_3D); \
VK_VIEW_TYPE_MAPPING(ImageViewCube, VK_IMAGE_VIEW_TYPE_CUBE); \
VK_VIEW_TYPE_MAPPING(ImageView1DArray, VK_IMAGE_VIEW_TYPE_1D_ARRAY); \
VK_VIEW_TYPE_MAPPING(ImageView2DArray, VK_IMAGE_VIEW_TYPE_2D_ARRAY); \
VK_VIEW_TYPE_MAPPING(ImageViewCubeArray, VK_IMAGE_VIEW_TYPE_CUBE_ARRAY); 


#define VK_VIEW_TYPE_MAPPING(format, vkformat) \
{case VkImageViewType::vkformat:return ResourceViewType::format;} 

#define VK_NON_DEF(vt) 

namespace Sakura::Graphics::Vk
{
    static const ResourceViewType Transfer(const VkImageViewType viewType)
    {
         switch (viewType)
        {
            VIEW_TYPE_MAPPER()
        default:
            Sakura::log::error("No valid Vk Image View Type found!");
            return ResourceViewType::ImageViewTypesCount;
        }
    }

} 


#undef VK_NON_DEF
#undef VK_VIEW_TYPE_MAPPING


#define VK_VIEW_TYPE_MAPPING(format, vkformat) \
{case ResourceViewType::format: \
return VkImageViewType::vkformat;}

#define VK_NON_DEF(vt)\
{case ResourceViewType::vt: \
std::string val##vt = "Vulkan View Type not supported: "; \
std::string fmt##vt = #vt; \
Sakura::log::warn(val##vt + fmt##vt); \
return VkImageViewType::VK_FORMAT_UNDEFINED;}

namespace Sakura::Graphics::Vk
{
    static const VkImageViewType Transfer(const ResourceViewType viewType)
    {
         switch (viewType)
        {
            VIEW_TYPE_MAPPER()
        default:
            Sakura::log::error("No valid Vk View Type found!");
            return VkImageViewType::VK_IMAGE_VIEW_TYPE_MAX_ENUM;
        }
    }

} 

#undef VIEW_TYPE_MAPPER
#undef VK_NON_DEF
#undef VK_VIEW_TYPE_MAPPING
