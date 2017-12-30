#include "Pause.h"

using namespace std;


Pause::Pause() : m_Pause(FALSE)
{
}

Pause::~Pause()
{
}


BOOL Pause::TogglePause()
{
	lock_guard<mutex> lock(this->m_Mutex);
	this->m_Pause = !this->m_Pause;
	if (!this->m_Pause)
		this->m_PauseCV.notify_all();

	return this->m_Pause;
}

void Pause::Resume()
{
	lock_guard<mutex> lock(this->m_Mutex);
	this->m_Pause = FALSE;
	this->m_PauseCV.notify_all();
}

BOOL Pause::IsPaused()
{
	return this->m_Pause;
}

void Pause::DoPause()
{
	while (this->m_Pause)
	{
		unique_lock<mutex> lock(this->m_Mutex);
		this->m_PauseCV.wait(lock);
		lock.unlock();
	}
}