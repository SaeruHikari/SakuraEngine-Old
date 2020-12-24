#include "RenderGraphWebGPU/RenderGraphWebGPU.h"

using namespace sakura::graphics;
using namespace sakura::graphics::webgpu;

GpuTexture::GpuTexture(const GpuTextureHandle handle, const webgpu::RenderDevice& dev,
	const TextureDescriptor& desc) noexcept
	:handle_(handle), desc_(desc)
{
	WGPUTextureDescriptor textureDesc = translate(desc);
	texture = wgpuDeviceCreateTexture(dev.device, &textureDesc);
	WGPUTextureViewDescriptor vdesc = {};

	WGPUTextureViewDimension d;
	vdesc.dimension = match(translate(desc.dimension));
	vdesc.arrayLayerCount = desc.array_layers;
	vdesc.aspect = WGPUTextureAspect_All;
	vdesc.baseArrayLayer = 0;
	vdesc.arrayLayerCount = 1;
	vdesc.baseMipLevel = 0;
	// TODO: mip-map initialize.
	vdesc.mipLevelCount = 1;
	vdesc.format = textureDesc.format;
	default_view = wgpuTextureCreateView(texture, &vdesc);
	if(!texture)
	{
		sakura::error("[RenderGraphWebGPU]: Failed to craete texture!");
	}
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
