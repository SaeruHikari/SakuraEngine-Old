#pragma once
#include "D3D12Queues.h"
#include <windows.h>
#include <tchar.h>
#include <wrl.h>
#include <d3d12.h>
#include "d3dx12.h"
using namespace Microsoft;
using namespace Microsoft::WRL;

namespace sakura::graphics::d3d12
{
	class RenderGraphD3D12API RenderDevice : public IRenderDevice
	{
	public:
		RenderDevice(const DeviceConfiguration& config);
		~RenderDevice();

		virtual bool valid(const RenderShaderHandle shader) const override;
		virtual sakura::string_view get_name() const override;
		EBackend backend() const override;
		
		virtual bool execute(const RenderPass&, const RenderPassHandle) override;
		virtual bool execute(const RenderGraph& graph_to_execute) override;

		void destroy_resource(const RenderShaderHandle to_destroy) override;
		RenderShaderHandle create_shader(const RenderShaderHandle handle, const ShaderDesc& desc) override;
		RenderBufferHandle create_buffer(const RenderBufferHandle handle, const BufferDesc& desc) override;
		RenderAttachmentHandle create_render_attachment(const RenderAttachmentHandle handle, const Attachment& desc) override;
		SwapChainHandle create_swap_chain(const SwapChainHandle handle, const SwapChainDesc& desc) override;
		RenderPipelineHandle create_render_pipeline(const RenderPipelineHandle handle, const RenderPipelineDesc& desc) override;

		virtual void terminate() override;

		bool available_async_compute() const;

		ComPtr<ID3D12Device4> d3d12_device;
		GraphicsQueue* graphics_queue;
	private:
		void create_graphics_queue();

	protected:
		bool async_available = false;
		const string name = "UnnamedD3DDevice";
		RenderDevice();
	};
}
