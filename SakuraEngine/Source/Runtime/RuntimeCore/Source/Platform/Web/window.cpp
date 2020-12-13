#include <system/Window.h>
#include <system/Log.h>
#include <emscripten/html5.h>

static sakura::vector<sakura::string> registed_Windows;
sakura::Window sakura::Window::create(const sakura::Window::desc& desc) noexcept
{
    int width, height;
   
    auto res_code = emscripten_get_canvas_element_size(desc.name.c_str(), &width, &height);
    if(res_code != EMSCRIPTEN_RESULT_SUCCESS)
    {
        sakura::error("[Window web]: failed to regist canvas as Window! result code: {}", res_code);
        sakura::Window result(nullptr);
        return result;
    }
    else
    {
        auto res_code_set = emscripten_set_canvas_element_size(desc.name.c_str(), desc.width, desc.height);
        if(res_code_set != EMSCRIPTEN_RESULT_SUCCESS)
        {
            sakura::error("[Window web]: failed to set canvas size as {}, {}! result code: {}", desc.width, desc.height, res_code);
        }
        registed_Windows.emplace_back(desc.name);
        sakura::Window result((void*)registed_Windows[registed_Windows.size() - 1].c_str());
        return result;
    }
}

void sakura::Window::destroy(const Window wind) noexcept
{
    sakura::warn("Can't destroy any Window on Web Backend!");
}

void sakura::Window::show(const Window wind) noexcept
{
    sakura::warn("Can't show any Window on Web Backend!");
}

void sakura::Window::hide(const Window wind) noexcept
{
    sakura::warn("Can't hide any Window on Web Backend!");
}

bool sakura::Window::valid(const Window wind) noexcept
{
    return true;
}

sakura::extent2d sakura::Window::extent(const Window wind) noexcept
{
	int width, height;
    auto res_code = emscripten_get_canvas_element_size(
        (const char*)(wind.hdl), &width, &height);
    if(res_code != EMSCRIPTEN_RESULT_SUCCESS)
    {
        sakura::error("[Window web]: failed to query canvas size! result code: {}", res_code);
        return {0, 0};
    }
    return {static_cast<uint32>(width), static_cast<uint32>(height)};
}
