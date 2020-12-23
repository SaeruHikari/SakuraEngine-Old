#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <activation.h>

#if _WIN32_WINNT >= 0x0605
#include <ShellScalingApi.h>
#endif

#pragma comment(lib, "winmm.lib")
#if _WIN32_WINNT >= 0x0605
#pragma comment(lib, "shcore.lib")
#endif

/*
 * Default window width (in pixels at 96 DPI; scaled for other sizes).
 */
#ifndef WINDOW_WIN_W
#define WINDOW_WIN_W 800
#endif

 /*
  * Default window height (in pixels at 96 DPI; scaled for other sizes).
  */
#ifndef WINDOW_WIN_H
#define WINDOW_WIN_H 450
#endif

#include <WinUser.h>