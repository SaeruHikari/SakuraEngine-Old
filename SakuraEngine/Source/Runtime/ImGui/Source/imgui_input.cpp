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
#include "System/Input.h"

namespace sakura
{
	static void imgui_update_mouse_and_buttons()
	{
        ImGuiIO& io = ImGui::GetIO();
        io.MouseHoveredViewport = 0;

        // [1]
        // Only when requested by io.WantSetMousePos: set OS mouse pos from Dear ImGui mouse pos.
        // (rarely used, mostly when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (io.WantSetMousePos)
        {
            input::set_cursor_pos(io.MousePos.x, io.MousePos.y);
        }
        else
        {
            io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        }

        // [2]
        // Set Dear ImGui mouse pos from OS mouse pos + get buttons. (this is the common behavior)
        int mouse_x_local, mouse_y_local;
        auto pos = input::cursor_pos();
        mouse_x_local = pos.x;
        mouse_y_local = pos.y;
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[0] = input::key_down(input::EMouseKey::LB);      
        io.MouseDown[1] = input::key_down(input::EMouseKey::RB);
        io.MouseDown[2] = input::key_down(input::EMouseKey::MB);

        // TODO: single-viewport only, add multi-viewport.
        io.MousePos = ImVec2((float)mouse_x_local, (float)mouse_y_local);
	}

	static bool imgui_update_cursor()
	{
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
            return false;

        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
        if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
        {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            input::set_cursor(input::EMouseCursor::Hide);
        }
        else
        {
            switch (imgui_cursor)
            {
	            case ImGuiMouseCursor_Arrow:        input::set_cursor(input::EMouseCursor::Arrow); break;
	            case ImGuiMouseCursor_TextInput:    input::set_cursor(input::EMouseCursor::TextInput); break;
	            case ImGuiMouseCursor_ResizeAll:    input::set_cursor(input::EMouseCursor::ResizeAll); break;
	            case ImGuiMouseCursor_ResizeEW:     input::set_cursor(input::EMouseCursor::ResizeEW); break;
	            case ImGuiMouseCursor_ResizeNS:     input::set_cursor(input::EMouseCursor::ResizeNS); break;
	            case ImGuiMouseCursor_ResizeNESW:   input::set_cursor(input::EMouseCursor::ResizeNESW); break;
	            case ImGuiMouseCursor_ResizeNWSE:   input::set_cursor(input::EMouseCursor::ResizeNWSE); break;
	            case ImGuiMouseCursor_Hand:         input::set_cursor(input::EMouseCursor::Hand); break;
	            case ImGuiMouseCursor_NotAllowed:   input::set_cursor(input::EMouseCursor::NotAllowed); break;
            }
            
        }
        return true;
	}
	
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
        using namespace sakura::input;
		
        ImGuiIO& io = ImGui::GetIO();
        // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
        io.KeyMap[ImGuiKey_Tab] = static_cast<int>(EKeyCode::Tab);
        io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(EKeyCode::Left);
        io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(EKeyCode::Right);
        io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(EKeyCode::Up);
        io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(EKeyCode::Down);
        io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(EKeyCode::PageUp);
        io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(EKeyCode::PageDown);
        io.KeyMap[ImGuiKey_Home] = static_cast<int>(EKeyCode::Home);
        io.KeyMap[ImGuiKey_End] = static_cast<int>(EKeyCode::End);
        io.KeyMap[ImGuiKey_Insert] = static_cast<int>(EKeyCode::Insert);
        io.KeyMap[ImGuiKey_Delete] = static_cast<int>(EKeyCode::Del);
        io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(EKeyCode::Backspace);
        io.KeyMap[ImGuiKey_Space] = static_cast<int>(EKeyCode::SpaceBar);
        io.KeyMap[ImGuiKey_Enter] = static_cast<int>(EKeyCode::Enter);
        io.KeyMap[ImGuiKey_Escape] = static_cast<int>(EKeyCode::Esc);
        io.KeyMap[ImGuiKey_KeyPadEnter] = static_cast<int>(EKeyCode::Enter);
        io.KeyMap[ImGuiKey_A] = static_cast<int>(EKeyCode::A);
        io.KeyMap[ImGuiKey_C] = static_cast<int>(EKeyCode::C);
        io.KeyMap[ImGuiKey_V] = static_cast<int>(EKeyCode::V);
        io.KeyMap[ImGuiKey_X] = static_cast<int>(EKeyCode::X);
        io.KeyMap[ImGuiKey_Y] = static_cast<int>(EKeyCode::Y);
        io.KeyMap[ImGuiKey_Z] = static_cast<int>(EKeyCode::Z);

		
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		viewport->PlatformHandle = window.handle();
		
        imgui_update_monitors(window);
	}


    void imgui_new_frame(Window window, float delta_time)
    {
        ImGuiIO& io = ImGui::GetIO();

        auto extent = window.extent();
        io.DisplaySize = { static_cast<float>(extent.width), static_cast<float>(extent.height) };

        // !! no Scalable Drawing Support.
        // TODO: Support.
		io.DisplayFramebufferScale = ImVec2(1, 1);

		// set delta time.
        io.DeltaTime = delta_time;

        // Update MousePosAndButtons
        // Update MouseCursor
        // Update Gamepads.

        imgui_update_mouse_and_buttons();
        imgui_update_cursor();
		
        ImGui::NewFrame();
    }

}
