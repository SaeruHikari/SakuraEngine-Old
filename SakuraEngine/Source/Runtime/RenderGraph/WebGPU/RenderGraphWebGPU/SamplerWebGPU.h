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

	class RenderGraphWebGPUAPI GpuSampler : public IGpuSampler
	{
	public:
		GpuSampler(const GpuSamplerHandle handle,
			webgpu::RenderDevice& dev, const SamplerDesc& desc) noexcept;
		~GpuSampler();

		RenderResourceHandle handle() const override;
		size_t size() const override;
		WGPUSampler sampler = nullptr;
	protected:
		GpuSamplerHandle handle_ = GenerationalId::UNINITIALIZED;
		SamplerDesc desc_;
	};

}
