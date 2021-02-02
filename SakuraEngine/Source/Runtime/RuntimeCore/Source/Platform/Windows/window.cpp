#include <RuntimeCore/RuntimeCore.h>
#include "Platform/Windows/Windows.h"

namespace sakura::windows
{
	//**************************** Windows Event Loop ****************************/

	/**
	 * Helper to toggle between windowed mode and fullscreen.
	 *
	 * \todo tested only on Windows 10 so far
	 *
	 * \param[in] hWnd main application window handle
	 * \param[in] exit \c true if the toggle can only \e exit fullscreen
	 */
	static void toggleFullscreen(HWND const hWnd, bool const exit = false) {
		/*
		 * Note: stores the *single* window state of the main application (and
		 * doesn't track that the window handle is the same one).
		 */
		static struct FSState {
			FSState() : fullscrn(false) {}
			bool fullscrn; // whether the last call set fullscreen
			LONG wndStyle; // the windowed mode style (to restore)
			LONG extStyle; // the windowed mode extended style (to restore)
			RECT origRect; // the windowed mode original size and position
		} state;
		if (state.fullscrn == false) {
			if (!exit) {
				/*
				 * Stores the current window styles, unsets the menu bar and any
				 * frame features, then fits the undecorated content to the
				 * nearest monitor bounds.
				 */
				state.wndStyle = GetWindowLong(hWnd, GWL_STYLE);
				state.extStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
				GetWindowRect(hWnd, &state.origRect);
				MONITORINFO info; info.cbSize = sizeof(info);
				if (GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &info)) {
					SetWindowLong(hWnd, GWL_STYLE, state.wndStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowLong(hWnd, GWL_EXSTYLE, state.extStyle & ~WS_EX_OVERLAPPEDWINDOW);
					SetWindowPos(hWnd, HWND_TOP, info.rcMonitor.left, info.rcMonitor.top,
						info.rcMonitor.right - info.rcMonitor.left,
						info.rcMonitor.bottom - info.rcMonitor.top, SWP_FRAMECHANGED);
					state.fullscrn = !state.fullscrn;
				}
			}
		}
		else {
			/*
			 * Restores styles and size.
			 */
			SetWindowLong(hWnd, GWL_STYLE, state.wndStyle);
			SetWindowLong(hWnd, GWL_EXSTYLE, state.extStyle);
			SetWindowPos(hWnd, NULL, state.origRect.left, state.origRect.top,
				state.origRect.right - state.origRect.left,
				state.origRect.bottom - state.origRect.top, SWP_FRAMECHANGED);
			state.fullscrn = !state.fullscrn;
		}
	}

	/**
	   * Queries a window's DPI.
	   *
	   * \param[in] hWnd window handle (or \c null for the default window)
	   * \return best guess at the DPI
	   */
	static UINT getWindowDpi(HWND const hWnd = NULL) {
		/*
		 * GetDpiForMonitor and associated calls are removed from modern SDKs, and
		 * newer API calls aren't available on older Windows. This works. Sort of.
		 * The fallback (seemingly) grabs the default monitor and ignores any
		 * updated scale preferences, but is better than creating a tiny window on
		 * hi-DPI systems.
		 *
		 * TODO: add GetDpiForMonitor for older Windows anyway
		 */
		UINT dpi = 0;
		if (hWnd && GetDpiForWindow != NULL) {
			dpi = GetDpiForWindow(hWnd);
		}
		else {
			if (GetDpiForSystem != NULL) {
				dpi = GetDpiForSystem();
			}
			else {
				HDC hdc = GetDC(hWnd);
				if (hdc) {
					dpi = std::max(GetDeviceCaps(hdc, LOGPIXELSX),
						GetDeviceCaps(hdc, LOGPIXELSY));
					ReleaseDC(hWnd, hdc);
				}
			}
		}
		if (dpi == 0) {
			dpi = USER_DEFAULT_SCREEN_DPI;
		}
		return dpi;
	}
	
	/**
	* Standard \c WndProc function for Windows events, passing key presses, mouse
	* events, etc., to the different \c impl internal functions.
	*
	* \param[in] hWnd window handle
	* \param[in[ uMsg message type
	*/
	LRESULT CALLBACK windowEvents(HWND const hWnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam) 
	{
		LRESULT lResult = 0;
		bool handled = false;
		if (auto rx_msg = sakura::Core::find_messenger(reinterpret_cast<const void*>(hWnd)); rx_msg)
		{
			std::tie(handled, lResult) = ((WinMessages*)rx_msg)->message(hWnd, uMsg, wParam, lParam);
		}


		switch (uMsg) {
		case WM_SIZE:
			/*
			 * See note about the swap in surfaceResize and WM_WINDOWPOSCHANGED,
			 * so look into a better way of doing this.
			 */
			if (wParam != SIZE_MINIMIZED) {
				//impl::surfaceResize(LOWORD(lParam), HIWORD(lParam), dpiScale);
			}
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		case WM_ERASEBKGND:
			/*
			 * Return non-zero to tell Windows we handled the erase (a no-op).
			 */
			return 1;
		case WM_WINDOWPOSCHANGED:
			/*
			 * Early on, when first experimenting with ANGLE, moving the window
			 * was causing smearing artifacts. The workaround was to recopy the
			 * colour buffer for every message. This no longer appears necessary
			 * for moving (at a guess, the DX11 backend changed) but resizing and
			 * DPI changes look better. The (SWP_NOSIZE | SWP_NOMOVE) corresponds
			 * to window activation and DPI/monitor changes.
			 *
			 * A true fix would be to redraw, not just swap buffers, allowing for
			 * live resizing. This is a lightweight compromise.
			 */
			if ((reinterpret_cast<WINDOWPOS*>(lParam)->flags & SWP_NOSIZE) == 0 ||
				(reinterpret_cast<WINDOWPOS*>(lParam)->flags & (SWP_NOSIZE | SWP_NOMOVE)) == (SWP_NOSIZE | SWP_NOMOVE))
			{
				/*
				 * Hmm, I'm not happy still with this. It looks to cause massive
				 * flickering when going from different DPI monitors in some
				 * configs. The swap is commented out for now.
				 *
				 * if (eglDisplay != EGL_NO_DISPLAY && eglSurface != EGL_NO_SURFACE) {
				 *     eglSwapBuffers(eglDisplay, eglSurface);
				 * }
				 */
			}
			break;
		case WM_KEYUP:
			/*
			 * F11 toggles between fullscreen and windowed mode; ESC exits
			 * fullscreen (but does nothing if windowed).
			 *
			 * TODO: other function keys for 4:3, 16:9, etc.?
			 */
			if (wParam == VK_F11) {
				toggleFullscreen(hWnd);
			}
			else {
				if (wParam == VK_ESCAPE) {
					toggleFullscreen(hWnd, true);
				}
			}
			break;
#ifdef WM_DPICHANGED
			/*
			 * Handles the DPI change when moving between monitors (Win8.1+). The
			 * incoming RECT contains the suggested size, which works better than
			 * taking the DPI and calculating (given, when switching between
			 * normal to high to normal, increasingly shrinks the window). It's
			 * still not 100%, but is at least consistent when moving between
			 * monitors.
			 *
			 * TODO: older Windows (pre-Win10 1703) appear to get the size calculation wrong?
			 */
		case WM_DPICHANGED: {
			const RECT* rect = reinterpret_cast<RECT*>(lParam);
			SetWindowPos(hWnd, NULL, rect->left, rect->top,
				rect->right - rect->left,
				rect->bottom - rect->top,
				SWP_NOZORDER | SWP_NOACTIVATE);
			return 0;
		}
#endif
#ifdef WM_GETDPISCALEDSIZE
						  /*
						   * Windows gets this wrong (still as of Feb 2018), so we calculate it
						   * manually. The original size and scale work because 'hWnd' is still at
						   * the old DPI and 'wParam' is the new. Return TRUE to say we handled it
						   * ('lParam' contains the returned size).
						   *
						   * TODO: grab the menu parameter
						   */
		case WM_GETDPISCALEDSIZE: {
			float scale = static_cast<float>(wParam) / getWindowDpi(hWnd);
			RECT rect;
			GetClientRect(hWnd, &rect);
			rect.right = static_cast<LONG>((rect.right - rect.left) * scale + 0.5f);
			rect.bottom = static_cast<LONG>((rect.bottom - rect.top) * scale + 0.5f);
			rect.left = 0;
			rect.top = 0;
			if (AdjustWindowRectExForDpi != NULL) {
				AdjustWindowRectExForDpi(&rect,
					GetWindowLong(hWnd, GWL_STYLE), FALSE,
					GetWindowLong(hWnd, GWL_EXSTYLE),
					static_cast<UINT>(wParam));
			}
			else {
				/*
				 * TODO: test this fallback on older Windows
				 */
				AdjustWindowRectEx(&rect,
					GetWindowLong(hWnd, GWL_STYLE), FALSE,
					GetWindowLong(hWnd, GWL_EXSTYLE));
			}
			SIZE* size = reinterpret_cast<SIZE*>(lParam);
			size->cx = rect.right - rect.left;
			size->cy = rect.bottom - rect.top;
			return TRUE;
		}
#endif
		}
		if (!handled)
		{
			lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		if (uMsg == WM_NCDESTROY) {
			sakura::Core::unbind(hWnd);
		}
		return lResult;
	}
}


sakura::pos2d sakura::Monitor::screen_to_client(Window window, const pos2d& inPos)
{
	POINT mouse_client_pos = {
		static_cast<LONG>(inPos.x), static_cast<LONG>(inPos.y)};
	::ScreenToClient(static_cast<HWND>(window.handle()), &mouse_client_pos);
	pos2d outPos = { mouse_client_pos.x, mouse_client_pos.y };
	return outPos;
}

sakura::Window sakura::Window::create(const desc& desc) noexcept
{
	unsigned winW = desc.width;
	unsigned winH = desc.height;
	
	if (SetProcessDpiAwarenessContext != NULL) {
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	}
	HWND window = NULL;
	WNDCLASS wndClass;
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = sakura::windows::windowEvents;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("AppWin");
	if (RegisterClass(&wndClass)) {
		/*
		 * On multi-monitor systems we're not guaranteed the correct DPI
		 * without a window handle, so we create but don't show a window at
		 * the default position, query its DPI, then destroy it.
		 */
		window = CreateWindow(
			wndClass.lpszClassName, TEXT(desc.name.c_str()), 0,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, wndClass.hInstance, NULL);
		UINT const dpi = sakura::windows::getWindowDpi(window);
		if (window) {
			DestroyWindow(window);
		}
		/*
		 * Create the window, adjusted using the best DPI information we can
		 * obtain (which depends on the OS version). Works with newer Win10
		 * onwards, and tries to do the right thing on older versions.
		 */
#if WINDOW_WIN_RESIZE
		DWORD dwWndStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
#else
		DWORD dwWndStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME);
#endif
		DWORD dwExtStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES;
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = MulDiv((winW) ? winW : WINDOW_WIN_W, dpi, USER_DEFAULT_SCREEN_DPI);
		rect.bottom = MulDiv((winH) ? winH : WINDOW_WIN_H, dpi, USER_DEFAULT_SCREEN_DPI);
		if (AdjustWindowRectExForDpi != NULL) {
			AdjustWindowRectExForDpi(&rect, dwWndStyle, FALSE, dwExtStyle, dpi);
		}
		else {
			AdjustWindowRectEx(&rect, dwWndStyle, FALSE, dwExtStyle);
		}
		winW = rect.right - rect.left;
		winH = rect.bottom - rect.top;
		window = CreateWindowEx(dwExtStyle,
			wndClass.lpszClassName, TEXT(desc.name.c_str()),
			dwWndStyle, CW_USEDEFAULT, CW_USEDEFAULT,
			winW, winH, NULL, NULL, wndClass.hInstance, NULL);
		if (window) {
			/*
			 * Accept dropped files (which requires user code to register for
			 * drop events) and grab the print screen key (which is missing
			 * from WM_KEYDOWN).
			 */
			DragAcceptFiles(window, FALSE);
			RegisterHotKey(window, VK_SNAPSHOT, 0, VK_SNAPSHOT);
			ShowWindow(window, SW_SHOWDEFAULT);
			sakura::Window res(window);
			sakura::Core::bind(res);
			return res;
		}
		UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
	}
	sakura::Window invalid;
	return invalid;//Error Create Window.
}

void sakura::Window::destroy(const Window wind) noexcept
{
	if (!wind.hdl)
		sakura::error("window error: destroy a window handle with null HWND!");
	else
	{
		DestroyWindow(HWND(wind.hdl));
	}
}

void sakura::Window::set_title(const Window window, const sakura::wstring& title) noexcept
{
	if (!window.hdl)
		sakura::error("window error: set a window title with null HWND!");
	else
	{
		SetWindowTextW(HWND(window.hdl), title.c_str());
	}
}

void sakura::Window::show(const Window wind) noexcept
{
	if (sakura::Window::valid(wind))
		ShowWindow(HWND(wind.hdl), SW_SHOWDEFAULT);
	else
		sakura::error("{window::show]: invalid window handle!");
}

void sakura::Window::hide(const Window wind) noexcept
{
	if(sakura::Window::valid(wind))
		ShowWindow(HWND(wind.hdl), SW_HIDE);
	else
		sakura::error("{window::hide]: invalid window handle!");
}

sakura::extent2d sakura::Window::extent(const Window wind) noexcept
{
	tagRECT rect;
	GetClientRect(HWND(wind.hdl), &rect);
	return { static_cast<uint32>(rect.right - rect.left), static_cast<uint32>(rect.bottom - rect.top) };
}

struct sakura::Window::rectangle sakura::Window::rect(const Window wind) noexcept
{
	tagRECT rect;
	GetWindowRect(HWND(wind.hdl), &rect);
	sakura::Window::rectangle ret = {};
	ret.bottom = static_cast<uint32>(rect.bottom);
	ret.left = static_cast<uint32>(rect.left);
	ret.top = static_cast<uint32>(rect.top);
	ret.right = static_cast<uint32>(rect.right);
	return ret;
}

bool sakura::Window::valid(const Window wind) noexcept
{
	return wind.hdl != nullptr;
}


