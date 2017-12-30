#pragma once
#ifndef ACTION_H
#define ACTION_H

#include "global.h"
#include "Pause.h"
#include <thread>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include "GameWindow.h"
#include "GameState.h"


class Action
{
public:
	Action(GameState *state, GameWindow *game, std::chrono::milliseconds interval, BOOL startsImmediately = FALSE);
	virtual ~Action();

	virtual void Start();
	virtual void Stop();
	virtual BOOL TogglePause();
	void SetInterval(std::chrono::milliseconds interval);

protected:
	GameState *m_State;
	GameWindow *m_Game;
	std::atomic_bool m_ExitFlagCheck;
	Pause m_Pause;

	virtual void OnExecute(GameState *state, GameWindow *game) = 0;

private:
	std::thread m_Thread;
	std::chrono::milliseconds m_Interval;
	std::condition_variable m_ExitFlag;
	std::mutex m_LoopMutex;
	BOOL m_Running;
	BOOL m_StartsImmediately;

	void Run();
};

#endif
