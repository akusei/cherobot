#pragma once
#ifndef WINDOWHOOK_H
#define WINDOWHOOK_H

#include "global.h"

#ifndef WINDOWHOOK_API
#define WINDOWHOOK_API extern "C" __declspec(dllexport) 
#endif

WINDOWHOOK_API LRESULT CALLBACK GetMessageHook(int code, WPARAM wParam, LPARAM lParam);

WINDOWHOOK_API BOOL StartBot(HWND botWindow, HWND targetWindow);
WINDOWHOOK_API BOOL StopBot();

#endif
