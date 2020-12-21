#pragma once
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
#include "dawn/dawn_wsi.h"
#else
#include <emscripten/html5_webgpu.h>
#endif
#include "RenderGraph/RenderGraphCore.h"

namespace sakura::graphics::webgpu
{
	class RenderDevice;

	class RenderGraphWebGPUAPI SwapChain : public ISwapChain
	{
	public:
		SwapChain(const SwapChainHandle handle,
			const webgpu::RenderDevice& dev, const SwapChainDesc& desc);
		~SwapChain();
		uint8 buffer_count() const override;
		extent2d extent() const override;
		Window window() const override;
		RenderObjectHandle handle() const override;
		ETextureFormat render_format() const override;
		bool present() override;

		WGPUSwapChain swapchain;
#ifndef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
		DawnSwapChainImplementation dawnSwapChain = {};
#endif
		WGPUTextureFormat _format;
		SwapChainHandle _handle;
		Window _window;
	protected:
		extent2d _extent;
		uint32 _buffer_count = 0;
		void initPlatformSpecific(const SwapChainHandle handle,
			const webgpu::RenderDevice& dev, const SwapChainDesc& desc);
	};
}
