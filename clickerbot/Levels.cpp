#include "Levels.h"

using namespace std;

//TODO: move boss check into end of execute?

Levels::Levels(GameState *state, GameWindow *game, chrono::milliseconds levelDelay, chrono::milliseconds bossDelay, DWORD rollbackLevels) :
	Action(state, game, chrono::milliseconds(0)),
	m_LevelDelay(levelDelay),
	m_BossDelay(bossDelay),
	m_RollbackLevels(rollbackLevels)
{
}

Levels::~Levels()
{
}

void Levels::Start()
{
	Action::Start();
	this->m_TimeOnLevel = chrono::high_resolution_clock::now();
}

void Levels::SetLevelDelay(chrono::milliseconds delay)
{
	this->m_LevelDelay = delay;
}

void Levels::SetOldBossDelay(chrono::milliseconds delay)
{
	this->m_BossDelay = delay;
}

void Levels::SetRollbackLevels(DWORD rollbackLevels)
{
	this->m_RollbackLevels = rollbackLevels;
}

void Levels::OnExecute(GameState *state, GameWindow *game)
{
	BOOL hasLevel = this->LevelAvailable(Levels::NEXT_LEVEL);
	BOOL bossLevel = this->IsBossLevel();
	BOOL oldBoss = (bossLevel && hasLevel);
	state->SetOldBoss(oldBoss);
	state->SetBoss(bossLevel);

	chrono::milliseconds minTime = this->m_LevelDelay;
	if (oldBoss)
		minTime = this->m_BossDelay;
	else if (bossLevel)
		minTime = Levels::BOSS_DELAY;

	chrono::high_resolution_clock::time_point currentTime = chrono::high_resolution_clock::now();
	chrono::milliseconds timeOnLevel = chrono::duration_cast<chrono::milliseconds>(currentTime - this->m_TimeOnLevel);

	if (!hasLevel && bossLevel && timeOnLevel >= minTime)
	{
		lock_guard<mutex> lock(state->GetLevelChangeMutex());
		this->Rollback();
		return;
	}

	if (!hasLevel || timeOnLevel < minTime)
		return;

	lock_guard<mutex> lock(state->GetLevelChangeMutex());

	game->LeftClick(Levels::NEXT_LEVEL.x, Levels::NEXT_LEVEL.y);
	this->m_TimeOnLevel = chrono::high_resolution_clock::now();
}

void Levels::Rollback()
{
	DWORD count = 0;
	while (this->LevelAvailable(Levels::PREV_LEVEL) && count < this->m_RollbackLevels)
	{
		if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
			return;

		this->m_State->DoPause();

		this->m_Game->LeftClick(Levels::BACK_CLICK.x, Levels::BACK_CLICK.y);
		this_thread::sleep_for(chrono::milliseconds(350));
		count++;
	}

	this->m_Game->LeftClick(Levels::FIRST_LEVEL.x, Levels::FIRST_LEVEL.y);
	this->m_TimeOnLevel = chrono::high_resolution_clock::now();
}

BOOL Levels::IsBossLevel() const
{
	COLORREF color1 = this->m_Game->GetPixel(Levels::BOSS_CHECK1.x, Levels::BOSS_CHECK1.y);
	COLORREF color2 = this->m_Game->GetPixel(Levels::BOSS_CHECK2.x, Levels::BOSS_CHECK2.y);
	return (color1 == 0x00CC6600 && color2 == 0x00000033);
}

BOOL Levels::LevelAvailable(POINT pos) const
{
	COLORREF color = this->m_Game->GetPixel(pos.x, pos.y);
	return (color == 0x00487AC6);
}
