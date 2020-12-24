#include "RenderGraphWebGPU/FenceWebGPU.h"
#include "RenderGraphWebGPU/RenderGraphWebGPU.h"

sakura::graphics::webgpu::Fence::Fence(const FenceHandle handle, RenderDevice& dev, const FenceDescriptor& desc) noexcept
	:_handle(handle)
{
	WGPUFenceDescriptor fenceDesc = {};
	fenceDesc.initialValue = desc.initial_value;
	_fence = wgpuQueueCreateFence(dev.default_queue, &fenceDesc);
}

sakura::graphics::webgpu::Fence::~Fence()
{
	wgpuFenceRelease(_fence);
}

const sakura::uint64 sakura::graphics::webgpu::Fence::completed_value() const
{
	return wgpuFenceGetCompletedValue(_fence);
}

sakura::graphics::RenderObjectHandle sakura::graphics::webgpu::Fence::handle() const
{
	return _handle;
}
