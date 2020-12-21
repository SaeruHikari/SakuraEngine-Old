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

	class RenderGraphWebGPUAPI Fence final : public IFence
	{
	public:
		Fence(const FenceHandle handle, RenderDevice& dev, const FenceDesc& desc) noexcept;
		~Fence();
		
		const uint64 completed_value() const;

		RenderObjectHandle handle() const override;
		WGPUFence _fence;
	protected:
		FenceHandle _handle;
	};
}
