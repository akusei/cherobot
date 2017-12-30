#pragma once
#ifndef HOOK_H
#define HOOK_H

#include "global.h"

#ifndef WINDOWHOOK_API
#define WINDOWHOOK_API extern "C" __declspec(dllexport) 
#endif

WINDOWHOOK_API BOOL StartBot(HWND botWindow, HWND targetWindow);
WINDOWHOOK_API BOOL StopBot();

WINDOWHOOK_API void SetUpgradeInterval(DWORD interval);
WINDOWHOOK_API BOOL ToggleLevels();
WINDOWHOOK_API BOOL ToggleUpgrades();
WINDOWHOOK_API void SetLevelDelay(DWORD interval);
WINDOWHOOK_API void SetOldBossDelay(DWORD interval);
WINDOWHOOK_API void SetRollbackCount(DWORD levelCount);

#endif
