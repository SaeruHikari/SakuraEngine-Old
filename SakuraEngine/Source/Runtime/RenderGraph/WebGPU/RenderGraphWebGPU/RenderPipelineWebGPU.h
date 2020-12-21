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

	class RenderGraphWebGPUAPI RenderPipeline : public IRenderPipeline
	{
	public:
		RenderPipeline(RenderPipelineHandle handle,
			const webgpu::RenderDevice& device, const RenderPipelineDesc& desc);
		~RenderPipeline();
		RenderObjectHandle handle() const override;

		sakura::vector<WGPUBindGroupLayout> bindingGroups;
		WGPURenderPipeline renderPipeline = nullptr;
	protected:
		RenderPipelineHandle _handle;
	};
}
