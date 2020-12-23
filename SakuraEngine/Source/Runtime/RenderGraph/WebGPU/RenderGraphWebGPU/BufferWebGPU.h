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
	class RenderDevice;

	class RenderGraphWebGPUAPI GPUBuffer : public IGpuBuffer
	{
	public:
		GPUBuffer(const GpuBufferHandle handle,
			const webgpu::RenderDevice& dev, const BufferDesc& desc) noexcept;
		virtual ~GPUBuffer();
		RenderResourceHandle handle() const override;
		size_t size() const override;

		BufferUsages usages() const override;
		WGPUBuffer buffer = nullptr;
	protected:
		BufferDesc desc_;
		GpuBufferHandle handle_;
	};
}
