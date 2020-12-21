#include <RenderGraphWebGPU/RenderGraphWebGPU.h>

sakura::graphics::webgpu::GPUBuffer::GPUBuffer(const RenderBufferHandle handle,
	const webgpu::RenderDevice& dev, const BufferDesc& desc) noexcept
	:_desc(desc), _handle(handle)
{
	WGPUBufferDescriptor buffDesc = {};
	buffDesc.usage = WGPUBufferUsage_CopyDst | translate(desc.usages, desc.access);
	buffDesc.size = _desc.length;
	_buffer = wgpuDeviceCreateBuffer(dev.device, &buffDesc);
	if(desc.data)
	{
		wgpuQueueWriteBuffer(
			dev.default_queue, _buffer, 0, desc.data, desc.length);
	}
}

sakura::graphics::webgpu::GPUBuffer::~GPUBuffer()
{
	wgpuBufferRelease(_buffer);
}

sakura::graphics::RenderResourceHandle sakura::graphics::webgpu::GPUBuffer::handle() const
{
	return _handle;
}

size_t sakura::graphics::webgpu::GPUBuffer::size() const
{
	return _desc.length;
}

sakura::graphics::BufferUsages sakura::graphics::webgpu::GPUBuffer::usages() const
{
	return _desc.usages;
}

