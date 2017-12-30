#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameWindow.h"
#include "Pause.h"
#include <string>
#include <atomic>
#include <mutex>


class GameState
{
public:
	GameState();
	~GameState();

	void Status(const std::string &status, BOOL noNewLine = FALSE);
	void Verbose(const std::string &text, BOOL noNewLine = FALSE);
	void Debug(const std::string &text, BOOL noNewLine = FALSE);

	BOOL TogglePause(const HWND &gameWindow);
	BOOL IsPaused();
	void DoPause();
	void Resume();

	void SetBoss(BOOL value);
	BOOL IsBoss() const;
	void SetOldBoss(BOOL value);
	BOOL IsOldBoss() const;
	std::mutex &GetLevelChangeMutex();
	void ToggleDebug();
	void ToggleVerbose();

private:
	Pause m_Pause;
	std::atomic_bool m_Boss;
	std::atomic_bool m_OldBoss;
	std::atomic_bool m_Verbose;
	std::atomic_bool m_Debug;
	std::atomic_bool m_DoingUpgrades;

	std::mutex m_OutputMutex;
	std::mutex m_LevelChangeMutex;
};

#endif
