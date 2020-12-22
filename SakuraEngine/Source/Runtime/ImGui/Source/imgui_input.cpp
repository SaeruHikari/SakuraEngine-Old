#include "imgui/imgui_input.h"
#include <windows.h>
// Using XInput library for gamepad (with recent Windows SDK this may leads to executables which won't run on Windows 7)
#ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
#include <XInput.h>
#else
#define IMGUI_IMPL_WIN32_DISABLE_LINKING_XINPUT
#endif
#if defined(_MSC_VER) && !defined(IMGUI_IMPL_WIN32_DISABLE_LINKING_XINPUT)
#pragma comment(lib, "xinput")
//#pragma comment(lib, "Xinput9_1_0")
#endif

namespace sakura
{
	static void imgui_update_monitors(Window window)
	{
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        platform_io.Monitors.resize(0);
        // has no multi-viewport support.
		int display_count = 1; //SDL_GetNumVideoDisplays();
        for (int n = 0; n < display_count; n++)
        {
            // Warning: the validity of monitor DPI information on Windows depends on the application DPI awareness settings, which generally needs to be set in the manifest or at runtime.
            ImGuiPlatformMonitor monitor;
            auto r = window.rect();
            auto sz = window.extent();
            monitor.MainPos = monitor.WorkPos = ImVec2((float)r.left, (float)r.top);
            monitor.MainSize = monitor.WorkSize = ImVec2((float)sz.width, (float)sz.height);
            monitor.WorkPos = monitor.WorkPos = ImVec2((float)r.left, (float)r.top);
            monitor.WorkSize = monitor.WorkSize = ImVec2((float)sz.width, (float)sz.height);
            monitor.DpiScale = 1;
            platform_io.Monitors.push_back(monitor);
        }
	}

	void imgui_bind_window(Window window)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		viewport->PlatformHandle = window.handle();
		
        imgui_update_monitors(window);
	}
}
