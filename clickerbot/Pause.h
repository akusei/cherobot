#pragma once
#ifndef PAUSE_H
#define PAUSE_H

#include "global.h"
#include <atomic>
#include <condition_variable>
#include <mutex>


class Pause
{
public:
	Pause();
	virtual ~Pause();

	BOOL TogglePause();
	void Resume();
	void DoPause();
	BOOL IsPaused();

protected:
	std::atomic_bool m_Pause;
	std::mutex m_Mutex;
	std::condition_variable m_PauseCV;
};

#endif
