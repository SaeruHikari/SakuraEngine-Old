#include "imgui/imgui_input.h"

#ifdef SAKURA_TARGET_PLATFORM_WIN
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
#endif

#include "System/Input.h"

namespace sakura::imgui
{
	static void imgui_update_mouse_and_buttons(Window window)
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

        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        io.MouseHoveredViewport = 0;
		
        auto pos = input::cursor_pos();
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[0] = input::key_down(input::EMouseKey::LB);      
        io.MouseDown[1] = input::key_down(input::EMouseKey::RB);
        io.MouseDown[2] = input::key_down(input::EMouseKey::MB);

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            // Multi-viewport mode: mouse position in OS absolute coordinates (io.MousePos is (0,0) when the mouse is on the upper-left of the primary monitor)
            // This is the position you can get with GetCursorPos(). In theory adding viewport->Pos is also the reverse operation of doing ScreenToClient().
            if (ImGui::FindViewportByPlatformHandle((void*)window.handle()) != NULL)
                io.MousePos = ImVec2((float)pos.x, (float)pos.y);
        }
        else
        {
            // Single viewport mode: mouse position in client window coordinates (io.MousePos is (0,0) when the mouse is on the upper-left corner of the app window.)
            // This is the position you can get with GetCursorPos() + ScreenToClient() or from WM_MOUSEMOVE.
            //if (focused_hwnd == g_hWnd)
            {
                auto mouse_client_pos = Monitor::screen_to_client(window, pos);
                io.MousePos = ImVec2((float)mouse_client_pos.x, (float)mouse_client_pos.y);
            }
        }
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

#ifdef SAKURA_TARGET_PLATFORM_WIN
    static BOOL CALLBACK ImGui_ImplWin32_UpdateMonitors_EnumFunc(HMONITOR monitor, HDC, LPRECT, LPARAM)
    {
        MONITORINFO info = { 0 };
        info.cbSize = sizeof(MONITORINFO);
        if (!::GetMonitorInfo(monitor, &info))
            return TRUE;
        ImGuiPlatformMonitor imgui_monitor;
        imgui_monitor.MainPos = ImVec2((float)info.rcMonitor.left, (float)info.rcMonitor.top);
        imgui_monitor.MainSize = ImVec2((float)(info.rcMonitor.right - info.rcMonitor.left), (float)(info.rcMonitor.bottom - info.rcMonitor.top));
        imgui_monitor.WorkPos = ImVec2((float)info.rcWork.left, (float)info.rcWork.top);
        imgui_monitor.WorkSize = ImVec2((float)(info.rcWork.right - info.rcWork.left), (float)(info.rcWork.bottom - info.rcWork.top));
        imgui_monitor.DpiScale = 1.f;//ImGui_ImplWin32_GetDpiScaleForMonitor(monitor);
        ImGuiPlatformIO& io = ImGui::GetPlatformIO();
        if (info.dwFlags & MONITORINFOF_PRIMARY)
            io.Monitors.push_front(imgui_monitor);
        else
            io.Monitors.push_back(imgui_monitor);
        return TRUE;
    }

    static void imgui_update_monitors()
    {
        ImGui::GetPlatformIO().Monitors.resize(0);
        ::EnumDisplayMonitors(NULL, NULL, ImGui_ImplWin32_UpdateMonitors_EnumFunc, NULL);
    }
#elif defined(SAKURA_TARGET_PLATFORM_MACOS)
    static void imgui_update_monitors()
    {
        ImGui::GetPlatformIO().Monitors.resize(0);
    }
#endif

	void bind_window(Window window)
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

        // RX on OS MessageBus.
        auto rx_os = sakura::Core::find_messenger(window.handle());
        rx_os->messages<OSKeyboardMessage>().subscribe([](auto m)
            {
                ImGuiIO& io = ImGui::GetIO();

                if (m.message == SM_KEYUP || m.message == SM_SYS_KEYUP)
                {
                    if ((uint32)m.key_code < 256)
                        io.KeysDown[(uint32)m.key_code] = 0;
                }
                else if (m.message == SM_KEYDOWN || m.message == SM_SYS_KEYDOWN)
                {
                    if ((uint32)m.key_code < 256)
                        io.KeysDown[(uint32)m.key_code] = 1;
                }
            });
#ifdef SAKURA_TARGET_PLATFORM_WIN
        windows::WinMessages* rx_win = static_cast<windows::WinMessages*>(rx_os);
        rx_win->messages_win<WM_CHAR>().
            subscribe([](auto m)
            {
                ImGuiIO& io = ImGui::GetIO();

                if (m.wParam > 0 && m.wParam < 0x10000)
                    io.AddInputCharacterUTF16((unsigned short)m.wParam);
            });

#endif
		
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		viewport->PlatformHandle = window.handle();
		
        imgui_update_monitors();
	}


    void new_frame(Window window, float delta_time)
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

        imgui_update_monitors();
        imgui_update_mouse_and_buttons(window);
        imgui_update_cursor();
		
        ImGui::NewFrame();
    }

}
