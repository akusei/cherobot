#include "Clicker.h"

using namespace std;


Clicker::Clicker(GameState *state, GameWindow *game) : 
	Action(state, game, std::chrono::milliseconds(0)),
	m_Speed(Clicker::SPEED),
	m_CurrentX(Clicker::MIN_X),
	m_CurrentY(Clicker::MIN_Y)
{
}


Clicker::~Clicker()
{
}

void Clicker::OnExecute(GameState *state, GameWindow *game)
{
	game->LeftClick(this->m_CurrentX, this->m_CurrentY, 25); //maybe make faster?
	this->m_CurrentX += this->m_Speed;
	if (this->m_CurrentX > Clicker::MAX_X || this->m_CurrentX < Clicker::MIN_X)
		this->m_Speed *= -1;
}