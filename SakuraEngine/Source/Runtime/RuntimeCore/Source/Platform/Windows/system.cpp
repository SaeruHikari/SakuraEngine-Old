#include "RuntimeCore/RuntimeCore.h"
#include <mmsystem.h>
#include <shellapi.h>
#if _WIN32_WINNT >= 0x0605
#include <ShellScalingApi.h>
#endif

/*
 * Tunes the Windows timer and sleep period. Sets both the minimum timer
 * resolution and the sleep time between frame updates. A value of zero will
 * give a glitch-free playback at the expense of CPU (and battery). 10 will
 * drop CPU usage to zero but struggle to maintain framerate (but an even
 * paced struggle). 1-5 is a good choice (2 being a good compromise).
 */
#ifndef WINDOW_SLEEP_PERIOD
#define WINDOW_SLEEP_PERIOD 2
#endif

bool sakura::Core::yield() noexcept
{
	bool running = true;
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			running = false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(WINDOW_SLEEP_PERIOD);
	return running;
}

void sakura::Core::finalize() noexcept
{
	
}

sakura::Core::handle sakura::Core::current_app_handle() noexcept
{
	return GetModuleHandle(NULL);
}
