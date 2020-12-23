#pragma once
#include <SakuraSTL.hpp>

namespace sakura
{
    struct RuntimeCoreAPI Monitor
    {
    	struct Info
    	{
            pos2d main_pos;
            pos2d work_pos;
            extent2d work_size;
            extent2d main_size;
    	};
    };
	
    // hdl of an actual window.
    struct RuntimeCoreAPI Window    
    {
    	struct rectangle
    	{
            uint32 bottom;
            uint32 top;
            uint32 left;
            uint32 right;
    	};
        struct desc
        {
            uint32 width = 0;
            uint32 height = 0;
            sakura::string name;
        };
        [[nodiscard]] static Window create(const desc& desc) noexcept;
    	
        static void destroy(const Window wind) noexcept;
        inline void destroy() noexcept  {destroy(*this);}

        static void set_title(const Window window, const sakura::wstring& title) noexcept;
        void set_title(const sakura::wstring& title) noexcept { return set_title(*this, title); }
    	
        static void show(const Window wind) noexcept;
        inline void show() noexcept { return show(*this); }
    	
        static void hide(const Window wind) noexcept;
        inline void hide() noexcept { return hide(*this); }
    	
        static extent2d extent(const Window wind) noexcept;
        inline extent2d extent() const noexcept { return extent(*this); }
    	
        static rectangle rect(const Window wind) noexcept;
        inline rectangle rect() noexcept { return rect(*this); }
    	
        static bool valid(const Window wind) noexcept;
        inline bool valid() noexcept { return valid(*this); }
    	inline constexpr Window() noexcept
    		:hdl(nullptr)
    	{
    		
    	}
    	inline void* handle() const noexcept
    	{
            return hdl;
    	}
    protected:
        inline explicit Window(void* hwnd) noexcept
            :hdl(hwnd)
        {

        }
        void* hdl = nullptr;
    };
	
}
