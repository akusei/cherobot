#include "hook.h"
#include "messages.h"
#include <iostream>
#include <string>
#include <CommCtrl.h>
#include "GameState.h"
#include "GameWindow.h"
#include "Clicker.h"
#include "GameState.h"
#include "Upgrades.h"
#include "Levels.h"
#include "Buffs.h"

using namespace std;

typedef BOOL(__cdecl *InstallHookPtr)(HWND botWindow, HWND targetWindow);
typedef BOOL(__cdecl *RemoveHookPtr)(void);

GameState *g_State = nullptr;
GameWindow *g_GameWindow = nullptr;
Upgrades *g_Upgrades = nullptr;
Clicker *g_Clicker = nullptr;
Levels *g_Levels = nullptr;
Buffs *g_Buffs = nullptr;
HMODULE g_HookLib = NULL;

RemoveHookPtr g_RemoveHook = nullptr;


WINDOWHOOK_API BOOL StartBot(HWND botWindow, HWND targetWindow)
{
	if (g_HookLib != NULL)
		return FALSE;

	g_HookLib = LoadLibrary("window-hook.dll");
	if (!g_HookLib)
		return FALSE;

	InstallHookPtr installHook = (InstallHookPtr)GetProcAddress(g_HookLib, "InstallHook");
	g_RemoveHook = (RemoveHookPtr)GetProcAddress(g_HookLib, "RemoveHook");
	if (!installHook || !g_RemoveHook)
	{
		FreeLibrary(g_HookLib);
		g_HookLib = NULL;
		return FALSE;
	}

	g_GameWindow = new GameWindow(botWindow, targetWindow);

	COLORREF color = 0;
	while (color != 0x00A2EB91) //start button green
		color = g_GameWindow->GetPixel(566, 260);

	if (!installHook(botWindow, targetWindow))
	{
		FreeLibrary(g_HookLib);
		g_HookLib = NULL;
		delete g_GameWindow;
		return FALSE;
	}

	color = g_GameWindow->GetPixel(507, 344);
	if (color == 0x003ED20F)
		g_GameWindow->LeftClick(507, 344);

	color = g_GameWindow->GetPixel(933, 75);
	COLORREF testColor = color;

	g_GameWindow->LeftClick(566, 276);

	while (color == testColor || color == 0)
		color = g_GameWindow->GetPixel(933, 75);

	if (color == 0x00FA775E)
		g_GameWindow->LeftClick(933, 75);

	g_State = new GameState();
	g_Upgrades = new Upgrades(g_State, g_GameWindow, chrono::milliseconds(30000));
	g_Clicker = new Clicker(g_State, g_GameWindow);
	g_Levels = new Levels(g_State, g_GameWindow, chrono::milliseconds(70000), chrono::milliseconds(120000), 8);
	g_Buffs = new Buffs(g_State, g_GameWindow);

	g_Upgrades->Start();
	g_Clicker->Start();
	g_Levels->Start();
	g_Buffs->Start();

	return TRUE;
}

WINDOWHOOK_API BOOL StopBot()
{
	if (!g_HookLib)
		return FALSE;

	g_RemoveHook();
	FreeLibrary(g_HookLib);
	g_HookLib = NULL;

	g_Upgrades->Stop();
	g_Clicker->Stop();
	g_Levels->Stop();
	g_Buffs->Stop();

	delete g_Upgrades;
	delete g_Clicker;
	delete g_Levels;
	delete g_Buffs;
	delete g_State;
	delete g_GameWindow;
	
	return TRUE;
}

WINDOWHOOK_API BOOL TogglePause()
{
	if (!g_GameWindow || !g_State || !g_HookLib)
		return FALSE;

	return g_State->TogglePause(g_GameWindow->GetGameWindow());
}

WINDOWHOOK_API void SetUpgradeInterval(DWORD interval)
{
	if (!g_Upgrades)
		return;

	g_Upgrades->SetInterval(std::chrono::milliseconds(interval));
}

WINDOWHOOK_API BOOL ToggleLevels()
{
	if (!g_Levels)
		return FALSE;

	return g_Levels->TogglePause();
}

WINDOWHOOK_API BOOL ToggleUpgrades()
{
	if (!g_Upgrades)
		return FALSE;

	return g_Upgrades->TogglePause();
}

WINDOWHOOK_API void SetLevelDelay(DWORD interval)
{
	if (!g_Levels)
		return;

	g_Levels->SetLevelDelay(std::chrono::milliseconds(interval));
}

WINDOWHOOK_API void SetOldBossDelay(DWORD interval)
{
	if (!g_Levels)
		return;

	g_Levels->SetOldBossDelay(std::chrono::milliseconds(interval));
}

WINDOWHOOK_API void SetRollbackCount(DWORD levelCount)
{
	if (!g_Levels)
		return;

	g_Levels->SetRollbackLevels(levelCount);
}