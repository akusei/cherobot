#pragma once
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "global.h"
#include <string>
#include <mutex>


class GameWindow
{
public:
	const static long WHEEL_FORWARD = 1;
	const static long WHEEL_BACKWARD = -1;

	GameWindow(const HWND botWindow, const HWND gameWindow);
	virtual ~GameWindow();

	void LeftClick(DWORD x, DWORD y, DWORD delayMillis = 100);
	void LeftClick(POINT pos, DWORD delayMillis = 100);
	void MouseWheel(DWORD distance, long direction);
	void KeyPress(UINT vkCode) const;
	COLORREF GetPixel(DWORD x, DWORD y) const;
	COLORREF GetPixel(const POINT &pos) const;
	DWORD GetCRC(RECT area) const;

	const HWND &GetBotWindow() const;
	const HWND &GetGameWindow() const;

protected:
	HWND m_GameWindow;
	HWND m_BotWindow;
	std::mutex m_ClickMutex;
};

#endif
