#include "Buffs.h"

using namespace std;


//TODO: Old Boss detection sometimes doesn't trigger before buffs so buffs are used on old boss mode
// maybe change old boss check to at time of check instead of in level select?

Buffs::Buffs(GameState *state, GameWindow *game) :
	Action(state, game, chrono::milliseconds(0))
{
	this->m_Buffs[0] = { 0x00089AF4, { 589, 153, 37, 35 }, '1' };
	this->m_Buffs[1] = { 0x0005DF38, { 589, 205, 37, 35 }, '2' };
	this->m_Buffs[2] = { 0x00085C2D, { 589, 257, 37, 35 }, '3' };
	this->m_Buffs[3] = { 0x00000000, { 589, 309, 37, 35 }, '4' };
	this->m_Buffs[4] = { 0x00000000, { 589, 361, 37, 35 }, '5' };
	this->m_Buffs[5] = { 0x00000000, { 589, 413, 37, 35 }, '6' };
	this->m_Buffs[6] = { 0x00000000, { 589, 465, 37, 35 }, '7' };
	this->m_Buffs[7] = { 0x00000000, { 589, 517, 37, 35 }, '8' };
	this->m_Buffs[8] = { 0x00000000, { 589, 569, 37, 35 }, '9' };
}

Buffs::~Buffs()
{
}

void Buffs::OnExecute(GameState *state, GameWindow *game)
{
	if (!state->IsBoss() || state->IsOldBoss())
		return;

	for (int i = 0; i < Buffs::BUFF_COUNT; i++)
		game->KeyPress((0x30 + (i + 1)));
		//if (game->GetCRC(this->m_Buffs[i].Area) == this->m_Buffs[i].CRC)
		//	game->KeyPress(this->m_Buffs[i].Shortcut);
}
