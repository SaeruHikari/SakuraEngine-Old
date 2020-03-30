#include "GraphResourceNode.h"

using namespace Sakura::Graphics;

namespace Sakura::RenderGraph
{
	SGraphResourceNode::SGraphResourceNode(GpuResource* _resource)
		:resource(_resource)
	{

	}

	SGraphBufferNode::SGraphBufferNode(GpuBuffer* buf)
		:SGraphResourceNode(buf)
	{

	}

	SGraphTextureNode::SGraphTextureNode(GpuTexture* tex)
		: SGraphResourceNode(tex)
	{

	}
}