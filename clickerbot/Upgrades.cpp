#include "Upgrades.h"
#include "Clicker.h"
#include <sstream>

using namespace std;

//TODO: one click on scrollbar moves the screen 4 pixels?
// 45 clicks to get to bottom = 45 / 5 = 9 heroes?
// 45 / 9 = 5 pixels?
// takes 45 clicks exactly to get to bottom no matter how many in list

//scroll to bottom
//check up to top point
//scroll up one click at a time
//check top point

Upgrades::Upgrades(GameState *state, GameWindow *game, std::chrono::milliseconds interval) :
	Action(state, game, interval),
	m_NeedsScroll(FALSE)
{
}

Upgrades::~Upgrades()
{
}

void Upgrades::OnExecute(GameState *state, GameWindow *game)
{
	if (state->IsBoss() || state->IsOldBoss())
		return;

	DWORD maxScrolls = (this->NeedsScroll()) ? Upgrades::CLICKS_TO_TOP : 1;

	lock_guard<mutex> lock(state->GetLevelChangeMutex());

	if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
		return;

	this->DoPerks(state, game, maxScrolls);

	if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
		return;

	this->DoUpgrades(state, game, maxScrolls);
}

void Upgrades::DoPerks(GameState *state, GameWindow *game, DWORD maxScrolls)
{
	if (maxScrolls > 1)
	{
		game->LeftClick(Upgrades::DOWN_SCROLL);
		this_thread::sleep_for(chrono::milliseconds(200));
	}

	for (int i = 0; i < maxScrolls; i++)
	{
		if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
			return;

		this->m_State->DoPause();

		DWORD y = Upgrades::UPGRADE_BOTTOM.y;
		while (y > Upgrades::UPGRADE_TOP.y)
		{
			if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
				return;

			this->m_State->DoPause();

			POINT pos = this->GetPerks(y);
			if (pos.x != 0 && pos.y != 0)
			{
				for (int i = 0; i < Upgrades::PERK_COUNT; i++)
				{
					if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
						return;

					this->m_State->DoPause();

					DWORD x = Upgrades::PERKS_X[i];
					if (this->IsPurchased(x, pos.y))
						continue;

					if (this->PerkAvailable(x, pos.y))
					{
						if (!this->IsAscension(x, pos.y))
						{
							game->LeftClick(x + 18, pos.y + 6);
							game->LeftClick(Clicker::MIN_X, Clicker::MIN_Y);
						}
					}
					else
					{
						break;
					}
				}
			}
			y--;
		}

		if (maxScrolls > 1)
		{
			game->MouseWheel(1, GameWindow::WHEEL_FORWARD);
			this_thread::sleep_for(chrono::milliseconds(300));
		}
	}

	if (maxScrolls > 1)
		game->LeftClick(Upgrades::UP_SCROLL);
}

void Upgrades::DoUpgrades(GameState *state, GameWindow *game, DWORD maxScrolls)
{
	if (maxScrolls > 1)
	{
		game->LeftClick(Upgrades::DOWN_SCROLL);
		this_thread::sleep_for(chrono::milliseconds(200));
	}

	for (int i = 0; i < maxScrolls; i++)
	{
		if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
			return;

		this->m_State->DoPause();

		DWORD y = Upgrades::UPGRADE_BOTTOM.y;
		while (y > Upgrades::UPGRADE_TOP.y)
		{
			if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
				return;

			this->m_State->DoPause();

			POINT pos = this->GetUpgrade(y);
			if (pos.x != 0 && pos.y != 0)
			{
				while (this->UpgradeAvailable(pos.x, pos.y))
				{
					if (this->m_ExitFlagCheck || this->m_Pause.IsPaused())
						return;

					this->m_State->DoPause();

					game->LeftClick(pos);
				}
			}
			y--;
		}

		if (maxScrolls > 1)
		{
			game->MouseWheel(1, GameWindow::WHEEL_FORWARD);
			this_thread::sleep_for(chrono::milliseconds(300));
		}
	}

	if (maxScrolls > 1)
		game->LeftClick(Upgrades::UP_SCROLL);
}

BOOL Upgrades::UpgradeAvailable(DWORD x, DWORD y) const
{
	COLORREF color = this->m_Game->GetPixel(x, y);
	return (color == 0x00fecb00);
}

BOOL Upgrades::NeedsScroll()
{
	if (this->m_NeedsScroll)
		return this->m_NeedsScroll;

	COLORREF color = this->m_Game->GetPixel(16, 611);
	this->m_NeedsScroll = (color == 0x00ffffcc);
	return this->m_NeedsScroll;
}

BOOL Upgrades::IsAscension(DWORD x, DWORD y) const
{
	COLORREF color = this->m_Game->GetPixel(x + 12, y + 6);
	return (color == 0x00ffffff);
}

POINT Upgrades::GetUpgrade(DWORD y) const
{
	POINT pos = {};
	COLORREF color1 = this->m_Game->GetPixel(87, y);
	COLORREF color2 = this->m_Game->GetPixel(87, y - 12);
	COLORREF color3 = this->m_Game->GetPixel(87, y + 46);
	if (((color1 == 0x00fecb00 && color2 == 0x00fecb00) ||
		(color1 == 0x007e6500 && color2 == 0x007e6500)) && color3 == 0x00fcf8c1)
	{
		pos.x = 87;
		pos.y = y - 12;
	}

	return pos;
}

POINT Upgrades::GetPerks(DWORD y) const
{
	POINT pos = this->GetUpgrade(y);
	if (pos.x == 0 && pos.y == 0)
		return pos;

	pos.x = 177;
	pos.y += 20;

	return pos;
}

BOOL Upgrades::PerkAvailable(DWORD x, DWORD y) const
{
	COLORREF color = this->m_Game->GetPixel(x + 18, y + 1);
	BYTE r = (color & 0x00FF0000) >> 16;
	BYTE g = (color & 0x0000FF00) >> 8;
	BYTE b = (color & 0x000000FF);

	return (r > 100 && g < 230 && b < 110); //(r > 100 && g < 200 && b < 110);
}

BOOL Upgrades::IsPurchased(DWORD x, DWORD y) const
{
	COLORREF color = this->m_Game->GetPixel(x + 18 , y);
	BYTE r = (color & 0x00FF0000) >> 16;
	BYTE g = (color & 0x0000FF00) >> 8;
	BYTE b = (color & 0x000000FF);

	return (r < 100 && g > 200 && b < 100);
}