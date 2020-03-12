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
 * @Date: 2020-03-11 20:52:05
 * @LastEditTime: 2020-03-12 21:11:16
 */
#include "FenceVk.h"
#include "../CGD_Vulkan.h"

using namespace Sakura::Graphics::Vk;

FenceVk::~FenceVk()
{
	vkDestroySemaphore(cgd.GetCGDEntity().device, timelineSemaphore, nullptr);
}
    
void FenceVk::Reset(void)
{
    
}

uint64 FenceVk::GetCompletedValue() const
{
    return 0;
}

FenceVk::FenceVk(const CGD_Vk& _cgd)
    :cgd(_cgd)
{
	VkSemaphoreTypeCreateInfo timelineCreateInfo;
	timelineCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
	timelineCreateInfo.pNext = NULL;
	timelineCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
	timelineCreateInfo.initialValue = 0;

	VkSemaphoreCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	createInfo.pNext = &timelineCreateInfo;
	createInfo.flags = 0;

	vkCreateSemaphore(_cgd.GetCGDEntity().device, &createInfo, NULL,
        &timelineSemaphore);
}

std::unique_ptr<Fence> CGD_Vk::AllocFence(void)
{
    Fence* result = new FenceVk(*this);
    return std::unique_ptr<Fence>(result);
}

void CGD_Vk::Wait(Fence* toWait, uint64 until) const
{
	FenceVk* fcVk = (FenceVk*)toWait;
	const uint64_t waitValue = until;

	VkSemaphoreWaitInfo waitInfo;
	waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
	waitInfo.pNext = NULL;
	waitInfo.flags = 0;
	waitInfo.semaphoreCount = 1;
	waitInfo.pSemaphores = &fcVk->timelineSemaphore;
	waitInfo.pValues = &waitValue;
    vkWaitSemaphores(entityVk.device, &waitInfo, UINT64_MAX);
}