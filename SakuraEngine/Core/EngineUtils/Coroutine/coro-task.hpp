#pragma once

namespace Sakura::coro
{
	struct task_base_t
	{
		task_base_t() = default;
	protected:
		friend scheduler_t;
		counted_ptr<state_base_t> _state;
	};
}