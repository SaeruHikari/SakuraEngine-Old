/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-12 16:28:09
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-13 17:14:35
 */
#include "Win32Window.h"
#include "Core/EngineUtils/log.h"
#include <windowsx.h>
#include <tchar.h>

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);

namespace Sakura
{
    void SWin32Window::OnDestroy(void)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        log::debug_info( "Window " + GetTitle() + " Destroyed.");
    #endif
        return;
    }

    int SWin32Window::OnCreate(void)
    {
    #ifdef SAKURA_DEBUG_EDITOR
        log::debug_info("Window " + GetTitle() + " Created.");
    #endif
        return 0;
    }

    int SWin32Window::____create(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPTSTR lpCmdLine, int nCmdShow)
    {
         // the handle for the window, filled by a function
        HWND hWnd;
        // this struct holds information for the window class
        WNDCLASSEX wc;

        // clear out the window class for use
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        // fill in the struct with the needed information
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = _T("WindowClass1");

        // register the window class
        RegisterClassEx(&wc);

        // create the window and use the result as the handle
        hWnd = CreateWindowEx(0,
                            _T("WindowClass1"),    // name of the window class
                            _T(desc.title.c_str()),   // title of the window
                            WS_OVERLAPPEDWINDOW,    // window style
                            300,    // x-position of the window
                            300,    // y-position of the window
                            (int)desc.width,    // width of the window
                            (int)desc.height,    // height of the window
                            NULL,    // we have no parent window, NULL
                            NULL,    // we aren't using menus, NULL
                            hInstance,    // application handle
                            NULL);    // used with multiple windows, NULL

        // display the window on the screen
        ShowWindow(hWnd, nCmdShow);

        // enter the main loop:

        // this struct holds Windows event messages
        MSG msg;

        // wait for the next message in the queue, store the result in 'msg'
        while(GetMessage(&msg, NULL, 0, 0))
        {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);
        }

        // return this part of the WM_QUIT message to Windows
        return msg.wParam;
    }

    SWin32Window::SWin32Window(const WindowDesc& desc)
        :Sakura::SWindow(desc)
    {
        const char* const myclass = "myclass";
        if (desc.bRegisterEx)
        {
            WNDCLASSEX wc;
            wc.cbSize = sizeof(WNDCLASSEX);
            /* Win 3.x */
            wc.style = CS_DBLCLKS;
            wc.lpfnWndProc = WindowProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = 0;
            wc.hInstance = GetModuleHandle(0);
            wc.hIcon = LoadIcon(0, IDI_APPLICATION);
            wc.hCursor = LoadCursor(0, IDC_ARROW);
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc.lpszMenuName = 0;
            wc.lpszClassName = myclass;
            /* Win 4.0 */
            wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

            if (RegisterClassEx(&wc))
            {
                HWND window = CreateWindowEx(0, myclass, desc.title.c_str(),
                    WS_OVERLAPPEDWINDOW, desc.AncorX, desc.AncorY,
                    desc.width, desc.height, 0, 0, GetModuleHandle(0), 0);

                if (window)
                {
                    ShowWindow(window, SW_SHOWDEFAULT);
                    MSG msg;
                    while (GetMessage(&msg, 0, 0, 0)) DispatchMessage(&msg);
                }
            }
        }
    }
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
            {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}