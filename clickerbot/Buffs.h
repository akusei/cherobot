#pragma once
#ifndef BUFFS_H
#define BUFFS

#include "Action.h"


class Buffs : public Action
{
public:
	Buffs(GameState *state, GameWindow *game);
	~Buffs();

protected:
	void OnExecute(GameState *state, GameWindow *game);

private:
	const static DWORD BUFF_COUNT = 9;

	struct BUFF
	{
		DWORD CRC;
		RECT Area;
		UCHAR Shortcut;
	};

	BUFF m_Buffs[Buffs::BUFF_COUNT];
};

#endif
