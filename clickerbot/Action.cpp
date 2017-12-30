#include "Action.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>

using namespace std;


Action::Action(GameState *state, GameWindow *game, chrono::milliseconds interval, BOOL startsImmediately) :
	m_Interval(interval),
	m_ExitFlagCheck(FALSE),
	m_State(state),
	m_Game(game),
	m_Running(FALSE),
	m_StartsImmediately(startsImmediately)
{
}


Action::~Action()
{
	this->Stop();
}

void Action::Start()
{
	this->m_Thread = thread(&Action::Run, this);
	this->m_Running = TRUE;
}

void Action::Stop()
{
	if (!this->m_Running)
		return;

	this->m_Pause.Resume();
	this->m_State->Resume();

	this->m_ExitFlagCheck = TRUE;
	this->m_ExitFlag.notify_one();

	try { this->m_Thread.join(); }
	catch (exception e) {}

	this->m_Running = FALSE;
}

BOOL Action::TogglePause()
{
	return this->m_Pause.TogglePause();
}

void Action::SetInterval(chrono::milliseconds interval)
{
	this->m_Interval = interval;
}

void Action::Run()
{
	unique_lock<mutex> loopLock(this->m_LoopMutex);
	chrono::milliseconds delay = (this->m_StartsImmediately) ? chrono::milliseconds(0) : this->m_Interval;

	while (!this->m_ExitFlag.wait_for(loopLock, delay, [&] { return m_ExitFlagCheck.load(); }))
	{
		this->m_Pause.DoPause();
		this->m_State->DoPause();

		this->OnExecute(this->m_State, this->m_Game);

		delay = this->m_Interval;
	}
}