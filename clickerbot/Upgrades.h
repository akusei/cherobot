#pragma once
#ifndef UPGRADES_H
#define UPGRADES_H

#include "Action.h"


class Upgrades : public Action
{
public:
	Upgrades(GameState *state, GameWindow *game, std::chrono::milliseconds interval);
	~Upgrades();

protected:
	void OnExecute(GameState *state, GameWindow *game);

private:
	const POINT UPGRADE_BOTTOM = { 90, 632 };
	const POINT UPGRADE_TOP = { 90, 197 };
	const POINT DOWN_SCROLL = { 547, 611 };
	const POINT UP_SCROLL = { 547, 203 };
	const POINT SCROLL_CHECK = { 547,191 };
	const DWORD CLICKS_TO_TOP = 15;//6; //14;
	const POINT UPGRADE_BASE = { 90, 197 };
	const DWORD OFFSET_X = 37;
	const POINT PERKS_BASE = { 140, 632 };
	const static DWORD PERK_COUNT = 7;
	const DWORD PERKS_X[Upgrades::PERK_COUNT] = {
		177,
		213,
		250,
		286,
		323,
		359,
		396
	};

	BOOL m_NeedsScroll;

	POINT GetUpgrade(DWORD y) const;
	BOOL UpgradeAvailable(DWORD x, DWORD y) const;
	BOOL NeedsScroll();
	BOOL PerkAvailable(DWORD x, DWORD y) const;
	BOOL IsPurchased(DWORD x, DWORD y) const;
	BOOL IsAscension(DWORD x, DWORD y) const;
	POINT GetPerks(DWORD y) const;

	void DoPerks(GameState *state, GameWindow *game, DWORD maxScrolls);
	void DoUpgrades(GameState *state, GameWindow *game, DWORD maxScrolls);
};

#endif
