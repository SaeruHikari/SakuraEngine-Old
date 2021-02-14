#include <dlfcn.h>
#include "RuntimeCore/RuntimeCore.h"

bool sakura::Core::yield() noexcept
{
	bool running = true;
    return running;
}

void sakura::Core::finalize() noexcept
{
	
}

sakura::Core::handle sakura::Core::current_app_handle() noexcept
{
	return dlopen(NULL, RTLD_NOW);
}