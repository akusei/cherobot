#include "windowhook.h"
#include "..\clickerbot\messages.h"
#include <iostream>
#include <string>
#include <CommCtrl.h>

using namespace std;

HHOOK g_MessageHook = NULL;
BOOL g_Created = FALSE;
HWND g_Window = NULL;
HWND g_WindowCover = NULL;
HWND g_BotWindow = NULL;
HWND g_TargetWindow = NULL;
HINSTANCE g_TargetInstance = NULL;
WNDPROC g_OldProc;

COLORREF g_RunningColor = RGB(0, 255, 0);
COLORREF g_PausedColor = RGB(255, 0, 0);
BOOL g_Paused = FALSE;


LRESULT CALLBACK OverlayProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			BeginPaint(hwnd, &paint);

			RECT rect = {};
			GetClientRect(hwnd, &rect);
			HPEN pen = CreatePen(PS_SOLID, 8, (g_Paused ? g_PausedColor : g_RunningColor));

			SelectObject(paint.hdc, pen);
			SelectObject(paint.hdc, GetStockObject(NULL_BRUSH));
			Rectangle(paint.hdc, rect.left, rect.top, rect.right, rect.bottom);

			EndPaint(hwnd, &paint);

			DeleteObject(pen);
			break;
		}

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
		case WM_LBUTTONDOWN:
			if ((wParam & 0x0000FF00) != 0x0E00)
			{
				if (!g_Paused)
					return 0;

				return DefSubclassProc(hWnd, uMsg, wParam, lParam);
			}
			wParam -= 0x0E00;
			break;

		case WM_LBUTTONUP:
		case WM_MOUSEWHEEL:
			if ((wParam & 0x0000FFFF) != 0x0E00)
			{
				if (!g_Paused)
					return 0;

				return DefSubclassProc(hWnd, uMsg, wParam, lParam);
			}
			wParam -= 0x0E00;
			break;

		case WM_MOUSEHOVER:
		case WM_MOUSEMOVE:
			if (!g_Paused)
				return 0;
			break;
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

WINDOWHOOK_API LRESULT CALLBACK GetMessageHook(int code, WPARAM wParam, LPARAM lParam)
{
	LRESULT retCode = CallNextHookEx(NULL, code, wParam, lParam);
	if (code < 0 || wParam != PM_REMOVE)
		return retCode;

	MSG *data = (MSG *)lParam;
	switch (data->message)
	{
		case WM_STARTBOT:
		{
			g_BotWindow = (HWND)data->wParam;
			g_TargetWindow = (HWND)data->lParam;
			g_Paused = FALSE;

			g_TargetInstance = (HINSTANCE)GetWindowLong(data->hwnd, GWL_HINSTANCE);
			RECT rect = {};
			GetClientRect(data->hwnd, &rect);

			WNDCLASS wndClass = {};
			wndClass.style = CS_HREDRAW | CS_VREDRAW;
			wndClass.lpfnWndProc = OverlayProc;
			wndClass.hInstance = g_TargetInstance;
			wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 255, 255));
			wndClass.lpszClassName = "OVERLAY";

			RegisterClass(&wndClass);
			g_Window = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, "OVERLAY", nullptr,
				WS_CHILDWINDOW, rect.left, rect.top, rect.right+1, rect.bottom+1, data->hwnd, nullptr, g_TargetInstance, nullptr);

			SetLayeredWindowAttributes(g_Window, RGB(0, 255, 255), 0, LWA_COLORKEY);

			ShowWindow(g_Window, SW_SHOW);
			UpdateWindow(g_Window);

			DWORD style = GetWindowLong(data->hwnd, GWL_STYLE);
			style &= ~WS_THICKFRAME;
			style &= ~WS_MINIMIZEBOX;
			style &= ~WS_MAXIMIZEBOX;
			SetWindowLong(data->hwnd, GWL_STYLE, style);

			SetWindowSubclass(data->hwnd, WndProc, 11141008, 0);
			break;
		}

		case WM_QUIT:
			RemoveWindowSubclass(data->hwnd, WndProc, 11141008);
			DestroyWindow(g_Window);
			UnregisterClass("OVERLAY", g_TargetInstance);
			PostMessage(g_BotWindow, WM_STOPBOT, NULL, NULL);
			break;

		case WM_STOPBOT:
		{
			RemoveWindowSubclass(data->hwnd, WndProc, 11141008);
			DWORD style = GetWindowLong(data->hwnd, GWL_STYLE);
			style |= WS_THICKFRAME;
			style |= WS_MINIMIZEBOX;
			style |= WS_MAXIMIZEBOX;
			SetWindowLong(data->hwnd, GWL_STYLE, style);
			DestroyWindow(g_Window);
			UnregisterClass("OVERLAY", g_TargetInstance);
			break;
		}

		case WM_PAUSEBOT:
			g_Paused = (BOOL)data->lParam;
			InvalidateRect(g_Window, NULL, TRUE);
			UpdateWindow(g_Window);
			break;
	}

	return retCode;
}

WINDOWHOOK_API BOOL InstallHook(HWND botWindow, HWND targetWindow)
{
	DWORD threadId = GetWindowThreadProcessId(targetWindow, nullptr);
	g_TargetWindow = targetWindow;

	g_MessageHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageHook, g_hInstance, threadId);
	if (g_MessageHook)
		PostMessage(targetWindow, WM_STARTBOT, (WPARAM)botWindow, (LPARAM)targetWindow);

	return (g_MessageHook != NULL);
}

WINDOWHOOK_API BOOL RemoveHook()
{
	if (g_MessageHook)
	{
		PostMessage(g_TargetWindow, WM_STOPBOT, NULL, NULL);
		Sleep(100);
	}

	return UnhookWindowsHookEx(g_MessageHook);
}
