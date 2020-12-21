#include "RenderGraphWebGPU/RenderGraphWebGPU.h"

using namespace sakura::graphics;
using namespace sakura::graphics::webgpu;

GpuTexture::GpuTexture(const RenderTextureHandle handle, const webgpu::RenderDevice& dev,
	const TextureDesc& desc) noexcept
	:handle_(handle), desc_(desc)
{
	WGPUTextureDescriptor textureDesc = translate(desc);
	texture = wgpuDeviceCreateTexture(dev.device, &textureDesc);
}

GpuTexture::~GpuTexture()
{
	wgpuTextureDestroy(texture);
}

RenderResourceHandle GpuTexture::handle() const
{
	return handle_;
}

size_t GpuTexture::size() const
{
	return 0;
}

TextureUsages GpuTexture::usages() const
{
	return desc_.usages;
}

sakura::uint32 GpuTexture::width() const
{
	return desc_.size.width;
}

sakura::uint32 GpuTexture::height() const
{
	return desc_.size.height;
}

sakura::uint32 GpuTexture::depth() const
{
	return desc_.size.depth;
}

ETextureFormat GpuTexture::render_format() const
{
	return desc_.format;
}
