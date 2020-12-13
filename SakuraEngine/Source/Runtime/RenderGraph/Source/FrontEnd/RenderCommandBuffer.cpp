#include "RenderGraph/RenderCommandBuffer.h"

using namespace sakura::graphics;

RenderCommandBuffer::RenderCommandBuffer(const sakura::string& _name, size_t size)
#ifdef _DEBUG
	:name(_name), buffers_allocator(size)
#else
	: buffers_allocator(size)
#endif
{
	commands.reserve(25);
	buffers_allocator.init();
}

RenderCommandBuffer::~RenderCommandBuffer()
{
	for (size_t i = 0u; i < commands.size(); i++)
		delete commands[i];
	buffers_allocator.reset();
}
