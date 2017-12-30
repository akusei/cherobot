#include "GameState.h"
#include "messages.h"
#include <iostream>

using namespace std;


GameState::GameState() : 
	m_OldBoss(FALSE), 
	m_Boss(FALSE),
	m_DoingUpgrades(FALSE)
{
}

GameState::~GameState()
{
}

void GameState::Status(const string &status, BOOL noNewLine)
{
	lock_guard<mutex> lock(this->m_OutputMutex);
	cout << status;
	if (!noNewLine)
		cout << endl;
}

void GameState::Verbose(const string &text, BOOL noNewLine)
{
	if (!this->m_Verbose)
		return;

	this->Status(text, noNewLine);
}

void GameState::Debug(const string &text, BOOL noNewLine)
{
	if (!this->m_Debug)
		return;

	this->Status(text, noNewLine);
}

BOOL GameState::TogglePause(const HWND &botWindow)
{
	BOOL paused = this->m_Pause.TogglePause();
	PostMessage(botWindow, WM_PAUSEBOT, NULL, paused);

	return paused;
}

BOOL GameState::IsPaused()
{
	return this->m_Pause.IsPaused();
}

void GameState::DoPause()
{
	this->m_Pause.DoPause();
}

void GameState::Resume()
{
	this->m_Pause.Resume();
}

void GameState::SetBoss(BOOL value)
{
	this->m_Boss = value;
}

BOOL GameState::IsBoss() const
{
	return this->m_Boss;
}

void GameState::SetOldBoss(BOOL value)
{
	this->m_OldBoss = value;
}

BOOL GameState::IsOldBoss() const
{
	return this->m_OldBoss;
}

mutex &GameState::GetLevelChangeMutex()
{
	return this->m_LevelChangeMutex;
}

void GameState::ToggleDebug()
{
	this->m_Debug = !this->m_Debug;
	string status = "DEBUG OUTPUT ";
	if (this->m_Debug)
		status.append("ENABLED");
	else
		status.append("DISABLED");

	this->Status(status);
}

void GameState::ToggleVerbose()
{
	this->m_Verbose = !this->m_Verbose;
	string status = "VERBOSE OUTPUT ";
	if (this->m_Verbose)
		status.append("ENABLED");
	else
		status.append("DISABLED");

	this->Status(status);
}


