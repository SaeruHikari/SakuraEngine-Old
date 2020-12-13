#pragma once
#include <RenderGraph/RenderCommand.h>
#include <RenderGraph/RenderGraph.h>

struct ID3D12CommandQueue;

namespace sakura::graphics::d3d12
{
	// this class is hidden to users.
	class RenderGraphD3D12API GraphicsQueue
	{
		friend class RenderDevice;

	protected:
		GraphicsQueue() = default;
		GraphicsQueue(ID3D12CommandQueue* queue);
		unique_ptr<ID3D12CommandQueue> command_queue;
	};
}
