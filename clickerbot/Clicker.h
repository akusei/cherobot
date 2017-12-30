#pragma once
#ifndef CLICKER_H
#define CLICKER_H

#include "Action.h"


class Clicker : public Action
{
public:
	const static DWORD MIN_X = 779;
	const static DWORD MIN_Y = 434;

	Clicker(GameState *state, GameWindow *game);
	~Clicker();

protected:
	void OnExecute(GameState *state, GameWindow *game);

private:
	const DWORD MAX_X = 929;
	const DWORD MAX_Y = 434;
	const DWORD SPEED = 3;

	DWORD m_CurrentX;
	DWORD m_CurrentY;
	int m_Speed;
};

#endif
