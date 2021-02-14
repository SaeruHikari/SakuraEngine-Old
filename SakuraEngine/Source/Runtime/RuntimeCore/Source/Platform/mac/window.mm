#include <RuntimeCore/RuntimeCore.h>
#include <System/Log.h>
#include "Platform/mac/Mac.h"
using namespace sakura;

#import <CoreVideo/CoreVideo.h>


//******************************** Public API ********************************/

sakura::Window sakura::Window::create(const desc& desc) noexcept
{
    sakura::Window res(nullptr);
	sakura::Core::bind(res);
	return res;
}

void sakura::Window::destroy(const Window wind) noexcept
{
    if (!wind.hdl)
		sakura::error("Window error: destroy a Window handle with null HWND!");
	else
	{
        
	}
}

void sakura::Window::show(const Window wind) noexcept
{
	if (sakura::Window::valid(wind))
	{

    }
	else
		sakura::error("[Window::show]: invalid Window handle!");
}

void sakura::Window::hide(const Window wind) noexcept
{
	if (sakura::Window::valid(wind))
	{
        
    }
	else
		sakura::error("[Window::hide]: invalid Window handle!");
}

bool sakura::Window::valid(const Window wind) noexcept
{
	return wind.hdl != nullptr;
}