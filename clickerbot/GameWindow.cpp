#include "GameWindow.h"
#include <thread>
#include <iostream>
#include <conio.h>


using namespace std;


GameWindow::GameWindow(const HWND botWindow, const HWND gameWindow) : m_BotWindow(botWindow), m_GameWindow(gameWindow)
{
}

GameWindow::~GameWindow()
{
}

void GameWindow::LeftClick(DWORD x, DWORD y, DWORD delayMillis)
{
	LPARAM lparam = MAKELPARAM(x, y);

	lock_guard<mutex> lock(this->m_ClickMutex);

	PostMessage(this->m_GameWindow, WM_LBUTTONDOWN, 0x0E00 | MK_LBUTTON, lparam);
	this_thread::sleep_for(chrono::milliseconds(delayMillis));
	PostMessage(this->m_GameWindow, WM_LBUTTONUP, 0x0E00, lparam);
}

void GameWindow::LeftClick(POINT pos, DWORD delayMillis)
{
	this->LeftClick(pos.x, pos.y, delayMillis);
}

void GameWindow::MouseWheel(DWORD distance, long direction)
{
	RECT rect = {};
	GetWindowRect(this->m_GameWindow, &rect);

	DWORD lParam = MAKELPARAM(rect.left + 568, rect.top + 320);
	DWORD wParam = MAKEWPARAM(0x0E00, (distance * WHEEL_DELTA) * direction);
	SendMessage(this->m_GameWindow, WM_MOUSEWHEEL, wParam, lParam);
}

void GameWindow::KeyPress(UINT vkCode) const
{
	UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
	LPARAM lparam1 = MAKELPARAM(1, scanCode);
	LPARAM lparam2 = MAKELPARAM(1, 0xC000 | scanCode);

	PostMessage(this->m_GameWindow, WM_KEYDOWN, vkCode, lparam1);
	this_thread::sleep_for(chrono::milliseconds(25));
	PostMessage(this->m_GameWindow, WM_KEYUP, vkCode, lparam2);
}

const HWND &GameWindow::GetBotWindow() const
{
	return this->m_BotWindow;
}

const HWND &GameWindow::GetGameWindow() const
{
	return this->m_GameWindow;
}

COLORREF GameWindow::GetPixel(DWORD x, DWORD y) const
{
	HDC hdc = GetDC(this->m_GameWindow);

	BYTE *bitPointer;
	HDC hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = 1;
	bitmap.bmiHeader.biHeight = 1;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = 4;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, 1, 1, hdc, x, y, SRCCOPY);

	COLORREF color = RGB(bitPointer[0], bitPointer[1], bitPointer[2]);

	DeleteObject(hBitmap2);
	DeleteObject(hdcTemp);
	ReleaseDC(this->m_GameWindow, hdc);

	return color;
}

COLORREF GameWindow::GetPixel(const POINT &pos) const
{
	return this->GetPixel(pos.x, pos.y);
}

DWORD GameWindow::GetCRC(RECT area) const
{
	HDC hdc = GetDC(this->m_GameWindow);

	DWORD bufferSize = (area.right * area.bottom) * 4;

	BYTE *bitPointer;
	HDC hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = area.right;
	bitmap.bmiHeader.biHeight = area.bottom;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = bufferSize;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, area.right, area.bottom, hdc, area.left, area.top, SRCCOPY);

	DWORD crc = 0;
	for (int i = 0; i < bufferSize; i += 4)
	{
		BYTE r = bitPointer[i];
		BYTE g = bitPointer[i + 1];
		BYTE b = bitPointer[i + 2];

		crc += (r + g) ^ ((g << 1) + b);
	}

	DeleteObject(hBitmap2);
	DeleteObject(hdcTemp);
	ReleaseDC(this->m_GameWindow, hdc);

	return crc;
}