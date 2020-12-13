#include "RenderGraph/RenderGraphCore.h"
#include <RenderGraphWebGPU/RenderGraphWebGPU.h>
#include "system/Log.h"

#if __EMSCRIPTEN_major__ == 1 && (__EMSCRIPTEN_minor__ < 40 || (__EMSCRIPTEN_minor__ == 40 && __EMSCRIPTEN_tiny__ < 1))
#error "Emscripten 1.40.1 or higher required"
#endif

#include <emscripten/html5_webgpu.h>

using namespace sakura::graphics::webgpu;
using namespace sakura::graphics;

void RenderDevice::initPlatformSpecific(const DeviceConfiguration& config)
{
	device = emscripten_webgpu_get_device();
	if(!device)
	{
		sakura::error("[RenderDeviceWebGPU]: Failed to get webgpu device!");
	}
}

