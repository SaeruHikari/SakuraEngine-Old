#pragma once
#include "RenderGraph/RenderGraphCore.h"
#ifdef SAKURA_TARGET_PLATFORM_EMSCRIPTEN
#include <webgpu/webgpu.h>
#else
#include <dawn/webgpu.h>
#include <dawn_native/DawnNative.h>
#endif

namespace sakura::graphics::webgpu
{
	class RenderGraphWebGPUAPI GpuTexture : public IGpuTexture
	{
	public:
		GpuTexture(const GpuTextureHandle handle,
			const webgpu::RenderDevice& dev, const TextureDesc& desc) noexcept;
		~GpuTexture();
		
		RenderResourceHandle handle() const override;
		size_t size() const override;
		TextureUsages usages() const override;
		uint32 width() const override;
		uint32 height() const override;
		uint32 depth() const override;
		ETextureFormat render_format() const override;
		
		WGPUTexture texture = nullptr;
	protected:
		TextureDesc desc_;
		GpuTextureHandle handle_;
	};

}
