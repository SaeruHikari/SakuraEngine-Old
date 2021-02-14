// Dawn Implementation of WebGPU SwapChain.
#include <RenderGraphWebGPU/RenderGraphWebGPU.h>
#include "System/Log.h"
#include <dawn/dawn_proc.h>
#include <dawn/webgpu_cpp.h>
#include <dawn_native/NullBackend.h>

#if __has_include("d3d12.h") || (_MSC_VER >= 1900)
#define DAWN_ENABLE_BACKEND_D3D12
#elif __has_include("vulkan/vulkan.h") && (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64))
#define DAWN_ENABLE_BACKEND_VULKAN
#elif defined(SAKURA_TARGET_PLATFORM_MACOS)
#define DAWN_ENABLE_BACKEND_METAL
#endif

#ifdef DAWN_ENABLE_BACKEND_D3D12
#include <dawn_native/D3D12Backend.h>
#endif
#ifdef DAWN_ENABLE_BACKEND_VULKAN
#include <dawn_native/VulkanBackend.h>
#include <vulkan/vulkan_win32.h>
#endif

#ifdef DAWN_ENABLE_BACKEND_METAL
#include <dawn_native/MetalBackend.h>
#endif

#pragma comment(lib, "dawn_native.lib")
#pragma comment(lib, "dawn_proc.lib")
#ifdef DAWN_ENABLE_BACKEND_VULKAN
#pragma comment(lib, "vulkan-1.lib")
#endif

using namespace sakura::graphics::webgpu;

#ifdef DAWN_ENABLE_BACKEND_VULKAN
static VkSurfaceKHR createVkSurface(WGPUDevice device, void* window) {
	VkSurfaceKHR surface = VK_NULL_HANDLE;
#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.hinstance = GetModuleHandle(NULL);
	info.hwnd = reinterpret_cast<HWND>(window);
	vkCreateWin32SurfaceKHR(
		dawn_native::vulkan::GetInstance(device),
		&info, nullptr, &surface);
#endif
	return surface;
}
#endif

WGPUBackendType RenderDevice::get_backend()
{
#ifdef DAWN_ENABLE_BACKEND_VULKAN
	return WGPUBackendType_Vulkan;
#else
	return WGPUBackendType_D3D12;
#endif
}

void SwapChain::initPlatformSpecific(const SwapChainHandle handle, const webgpu::RenderDevice& dev,
	const SwapChainDescriptor& desc)
{
	auto backend = RenderDevice::get_backend();
	auto& window = desc.window;
	switch (backend) {
#ifdef DAWN_ENABLE_BACKEND_D3D12
	case WGPUBackendType_D3D12:
		if (dawnSwapChain.userData == nullptr) {
			dawnSwapChain = dawn_native::d3d12::CreateNativeSwapChainImpl(
				dev.device, reinterpret_cast<HWND>(window.handle()));
			_format = dawn_native::d3d12::GetNativeSwapChainPreferredFormat(&dawnSwapChain);
		}
		break;
#endif
#ifdef DAWN_ENABLE_BACKEND_VULKAN
	case WGPUBackendType_Vulkan:
		if (dawnSwapChain.userData == nullptr) {
			dawnSwapChain = dawn_native::vulkan::CreateNativeSwapChainImpl(
				dev.device, createVkSurface(dev.device, window.handle()));
			_format = dawn_native::vulkan::GetNativeSwapChainPreferredFormat(&dawnSwapChain);
		}
		break;
#endif
#ifdef DAWN_ENABLE_BACKEND_METAL
	case WGPUBackendType_Metal:
		if (dawnSwapChain.userData == nullptr) {
			//dawnSwapChain = dawn_native::metal::CreateNativeSwapChainImpl(
			//	dev.device, createVkSurface(dev.device, window.handle()));
			//_format = dawn_native::metal::GetNativeSwapChainPreferredFormat(&dawnSwapChain);
		}
		break;
#endif
	default:
		if (dawnSwapChain.userData == nullptr) {
			dawnSwapChain = dawn_native::null::CreateNativeSwapChainImpl();
			_format = WGPUTextureFormat_Undefined;
		}
		break;
	}

	WGPUSwapChainDescriptor swapDesc = {};
	swapDesc.implementation = reinterpret_cast<uintptr_t>(&dawnSwapChain);
	swapDesc.presentMode = translate(desc.present_mode);
	
	swapchain = wgpuDeviceCreateSwapChain(dev.device, nullptr, &swapDesc);
	/*
	 * Currently failing on hi-DPI (with Vulkan).
	 */
	_extent = sakura::Window::extent(window);
	wgpuSwapChainConfigure(
		swapchain, _format, WGPUTextureUsage_OutputAttachment, 
		_extent.width, _extent.height);
}
