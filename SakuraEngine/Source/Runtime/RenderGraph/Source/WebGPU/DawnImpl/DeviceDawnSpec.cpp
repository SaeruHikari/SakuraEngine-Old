#include <RenderGraphWebGPU/RenderGraphWebGPU.h>
#include <dawn_platform/dawn_platform_export.h>
#include <dawn/webgpu_cpp.h>
#include <dawn/dawn_proc.h>
#include <dawn/dawn_proc_table.h>
#include "System/Log.h"

namespace impl
{
	/**
	 * Analogous to the browser's \c GPU.requestAdapter().
	 * \n
	 * The returned \c Adapter is a wrapper around the underlying Dawn adapter (and
	 * owned by the single Dawn instance).
	 *
	 * \todo we might be interested in whether the \c AdapterType is discrete or integrated for power-management reasons
	 *
	 * \param[in] type1st first choice of \e backend type (e.g. \c WGPUBackendType_D3D12)
	 * \param[in] type2nd optional fallback \e backend type (or \c WGPUBackendType_Null to pick the first choice or nothing)
	 * \return the best choice adapter or an empty adapter wrapper
	 */
	dawn_native::Adapter requestAdapter(WGPUBackendType type1st, WGPUBackendType type2nd = WGPUBackendType_Null) {
		static dawn_native::Instance instance;
		instance.DiscoverDefaultAdapters();
		wgpu::AdapterProperties properties;
		std::vector<dawn_native::Adapter> adapters = instance.GetAdapters();
		for (auto it = adapters.begin(); it != adapters.end(); ++it) {
			it->GetProperties(&properties);
			if (static_cast<WGPUBackendType>(properties.backendType) == type1st) {
				return *it;
			}
		}
		if (type2nd) {
			for (auto it = adapters.begin(); it != adapters.end(); ++it) {
				it->GetProperties(&properties);
				if (static_cast<WGPUBackendType>(properties.backendType) == type2nd) {
					return *it;
				}
			}
		}
		return dawn_native::Adapter();
	}
}

namespace impl
{
	/**
	 * Dawn error handling callback (adheres to \c WGPUErrorCallback).
	 *
	 * \param[in] message error string
	 */
	static void printError(WGPUErrorType /*type*/, const char* message, void*) {
		sakura::error("[Dawn]: {}", message);
	}
}

namespace sakura::graphics::webgpu
{
	void RenderDevice::initPlatformSpecific(const DeviceConfiguration& config)
	{
		WGPUBackendType type = get_backend();
		/*
		 * First go at this. We're only creating one global device/swap chain so far.
		 */
		adapter = impl::requestAdapter(type);
		if (adapter) {
			wgpu::AdapterProperties properties;
			adapter.GetProperties(&properties);
			auto backend = static_cast<WGPUBackendType>(properties.backendType);
			device = adapter.CreateDevice();
			
			DawnProcTable procs(dawn_native::GetProcs());
			procs.deviceSetUncapturedErrorCallback(device, impl::printError, nullptr);
			dawnProcSetProcs(&procs);
		}
	}
}
