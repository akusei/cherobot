#pragma once
#ifndef LEVELS_H
#define LEVELS_H

#include "Action.h"


class Levels : public Action
{
public:
	Levels(GameState *state, GameWindow *game, std::chrono::milliseconds levelDelay, std::chrono::milliseconds bossDelay, DWORD rollbackLevels);
	~Levels();

	void Start();
	void SetLevelDelay(std::chrono::milliseconds delay);
	void SetOldBossDelay(std::chrono::milliseconds delay);
	void SetRollbackLevels(DWORD rollbackLevels);

protected:
	void OnExecute(GameState *state, GameWindow *game);

private:
	const POINT NEXT_LEVEL = { 894, 58 };
	const POINT PREV_LEVEL = { 764, 58 };
	const POINT BACK_CLICK = { 669, 38 };
	const POINT FIRST_LEVEL = { 847, 43 };
	const POINT BOSS_CHECK1 = { 824, 153 };
	const POINT BOSS_CHECK2 = { 815, 172 };
	const std::chrono::milliseconds BOSS_DELAY = std::chrono::milliseconds(30000);

	std::chrono::milliseconds m_LevelDelay;
	std::chrono::milliseconds m_BossDelay;
	std::chrono::high_resolution_clock::time_point m_TimeOnLevel;
	std::chrono::milliseconds m_CurrentDelay;
	DWORD m_RollbackLevels;

	void Rollback();
	BOOL IsBossLevel() const;
	BOOL LevelAvailable(POINT pos) const;
};

#endif
