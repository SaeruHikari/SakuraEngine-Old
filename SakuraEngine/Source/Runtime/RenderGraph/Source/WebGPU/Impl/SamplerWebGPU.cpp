#include "RenderGraphWebGPU/RenderGraphWebGPU.h"

using namespace sakura;
using namespace sakura::graphics;

webgpu::GpuSampler::GpuSampler(const GpuSamplerHandle handle, webgpu::RenderDevice& dev,
	const SamplerDesc& desc) noexcept
	:handle_(handle), desc_(desc)
{
	WGPUSamplerDescriptor sampler_desc = {};
	sampler_desc = translate(desc);
	sampler = wgpuDeviceCreateSampler(dev.device, &sampler_desc);
}

webgpu::GpuSampler::~GpuSampler()
{
	wgpuSamplerRelease(sampler);
}

RenderResourceHandle webgpu::GpuSampler::handle() const
{
	return handle_;
}

size_t webgpu::GpuSampler::size() const
{
	return size_t(-1);
}
