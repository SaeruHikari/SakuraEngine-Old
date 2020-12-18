#pragma once
#include <RenderGraph/RenderCommand.h>
#include "Allocators/LinearAllocator.h"
#include "TaskSystem/TaskSystem.h"

namespace sakura::graphics
{
	struct RenderGraphAPI RenderCommandBuffer
	{
		RenderCommandBuffer(const sakura::string& name, size_t size = 1024);
		~RenderCommandBuffer();
		
		//RenderCommandBuffer(const RenderCommandBuffer&) = default;
		//RenderCommandBuffer& operator=(const RenderCommandBuffer&) = default;
		
		RenderCommandBuffer(RenderCommandBuffer&&) = default;
		
		template<typename Command, typename... Args>
		bool enqueue(Args&&... args);

		auto begin() const
		{
			return commands.begin();
		}
		auto end() const
		{
			return commands.end();
		}
		bool reset()
		{
			for (size_t i = 0u; i < commands.size(); i++)
				commands[i]->~RenderCommand();
			commands.resize(0);
			buffers_allocator.reset();
			return true;
		}
		bool valid() const
		{
			return true;
		}
	protected:
#ifdef _DEBUG
		sakura::string name = "none";
#endif
		sakura::vector<RenderCommand*> commands;
		linear_allocator buffers_allocator = linear_allocator(512);
	};

	template<typename Command, typename... Args>
	bool RenderCommandBuffer::enqueue(Args&&... args)
	{
		static_assert(std::is_base_of_v<RenderCommand, Command>,
			"Can only enqueue render_commands");
		void* mem = buffers_allocator.allocate(sizeof(Command));
		if(mem == nullptr)
		{
			assert(0 && "render_command_buffer");
		}
		commands.emplace_back(new (mem)Command(std::forward<Args>(args)...));
		return (mem != nullptr);
	}
}
