/*
 * Copyright 2010-2013 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SoldierDiaryState.h"
#include "SoldierInfoState.h"
#include "SoldierDiaryTotalsState.h"
#include <string>
#include "../Engine/Game.h"
#include "../Resource/ResourcePack.h"
#include "../Engine/Language.h"
#include "../Engine/Palette.h"
#include "../Engine/Options.h"
#include "../Geoscape/AllocatePsiTrainingState.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Base.h"
#include "../Savegame/Soldier.h"
#include "../Savegame/Craft.h"
#include "../Savegame/SoldierDiary.h"
#include "../Ruleset/RuleCraft.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Soldiers screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
SoldierDiaryTotalsState::SoldierDiaryTotalsState(Game *game, Base *base, size_t soldier, SoldierDiaryState *soldierDiaryState) : State(game), _base(base), _soldier(soldier), _soldierDiaryState(soldierDiaryState)
{
	// Create objects
	_window = new Window(this, 320, 200, 0, 0);
	_btnOk = new TextButton(96, 16, 216, 176);
	_btnPrev = new TextButton(28, 14, 8, 8);
	_btnNext = new TextButton(28, 14, 284, 8);
	_txtTitle = new Text(310, 16, 5, 8);
	_txtRace = new Text(76, 9, 8, 24);
	_txtRank = new Text(76, 9, 84, 24);
	_txtWeapon = new Text(76, 9, 160, 24);
	_txtUFO = new Text(76, 9, 236, 24);
	_lstRace = new TextList(76, 140, 8, 33);
	_lstRank = new TextList(76, 140, 84, 33);
	_lstWeapon = new TextList(76, 140, 160, 33);
	_lstUFO = new TextList(76, 140, 236, 33);

	// Set palette
	_game->setPalette(_game->getResourcePack()->getPalette("BACKPALS.DAT")->getColors(Palette::blockOffset(2)), Palette::backPos, 16);

	add(_window);
	add(_btnOk);
	add(_btnPrev);
	add(_btnNext); 
	add(_txtTitle);
	add(_txtRace);
	add(_txtRank);
	add(_txtWeapon);
	add(_txtUFO);
	add(_lstRace);
	add(_lstRank);
	add(_lstWeapon);
	add(_lstUFO);

	centerAllSurfaces();

	// Set up objects
	_window->setColor(Palette::blockOffset(15)+1);
	_window->setBackground(_game->getResourcePack()->getSurface("BACK02.SCR"));

	_btnOk->setColor(Palette::blockOffset(13)+10);
	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&SoldierDiaryTotalsState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&SoldierDiaryTotalsState::btnOkClick, (SDLKey)Options::getInt("keyCancel"));

	_btnPrev->setColor(Palette::blockOffset(15)+6);
	_btnPrev->setText(L"<<");
	_btnPrev->onMouseClick((ActionHandler)&SoldierDiaryTotalsState::btnPrevClick);

	_btnNext->setColor(Palette::blockOffset(15)+6);
	_btnNext->setText(L">>");
	_btnNext->onMouseClick((ActionHandler)&SoldierDiaryTotalsState::btnNextClick);

	_txtTitle->setColor(Palette::blockOffset(13)+10);
	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	// Text is decided in init()

	_txtRace->setColor(Palette::blockOffset(15)+1);
	_txtRace->setText(tr("STR_RACE"));

	_txtRank->setColor(Palette::blockOffset(15)+1);
	_txtRank->setText(tr("STR_RANK"));

	_txtWeapon->setColor(Palette::blockOffset(15)+1);
	_txtWeapon->setText(tr("STR_WEAPON"));

	_txtUFO->setColor(Palette::blockOffset(15)+1);
	_txtUFO->setText(tr("STR_UFO"));

	_lstRace->setColor(Palette::blockOffset(13)+10);
	_lstRace->setArrowColor(Palette::blockOffset(15)+1);
	_lstRace->setColumns(2, 66, 6);
	_lstRace->setBackground(_window);

	_lstRank->setColor(Palette::blockOffset(13)+10);
	_lstRank->setArrowColor(Palette::blockOffset(15)+1);
	_lstRank->setColumns(2, 66, 6);
	_lstRank->setBackground(_window);

	_lstWeapon->setColor(Palette::blockOffset(13)+10);
	_lstWeapon->setArrowColor(Palette::blockOffset(15)+1);
	_lstWeapon->setColumns(2, 66, 6);
	_lstWeapon->setBackground(_window);

	_lstUFO->setColor(Palette::blockOffset(13)+10);
	_lstUFO->setArrowColor(Palette::blockOffset(15)+1);
	_lstUFO->setColumns(2, 66, 6);
	_lstUFO->setBackground(_window);

	init(); // Populate the list
}

/**
 *
 */
SoldierDiaryTotalsState::~SoldierDiaryTotalsState()
{

}

/**
 *  Clears all the variables and reinitializes the list of medals for the soldier.
 *
 */
void SoldierDiaryTotalsState::init()
{
	Soldier *s = _base->getSoldiers()->at(_soldier);
	_txtTitle->setText(s->getName());
	_lstRace->clearList();
	_lstRank->clearList();
	_lstWeapon->clearList();
	_lstUFO->clearList();
	std::map<std::string, int> _raceTotals = s->getDiary()->getAlienRaceTotal();
	std::map<std::string, int> _rankTotals = s->getDiary()->getAlienRankTotal();
	std::map<std::string, int> _weaponTotals = s->getDiary()->getWeaponTotal();
	std::map<std::string, int> _UFOTotals = s->getDiary()->getUFOTotal();
	// int _scoreTotal = s->getDiary()->getScoreTotal();

	for (std::map<std::string, int>::const_iterator i = _raceTotals.begin() ; i != _raceTotals.end() ; ++i)
	{
		std::wstringstream ss1, ss2;

		ss1 << tr((*i).first.c_str());
		ss2 << (*i).second;
		_lstRace->addRow(2, ss1.str().c_str(), ss2.str().c_str());
	}

	for (std::map<std::string, int>::const_iterator i = _rankTotals.begin() ; i != _rankTotals.end() ; ++i)
	{
		std::wstringstream ss1, ss2;

		ss1 << tr((*i).first.c_str());
		ss2 << (*i).second;
		_lstRank->addRow(2, ss1.str().c_str(), ss2.str().c_str());
	}

	for (std::map<std::string, int>::const_iterator i = _weaponTotals.begin() ; i != _weaponTotals.end() ; ++i)
	{
		std::wstringstream ss1, ss2;

		ss1 << tr((*i).first.c_str());
		ss2 << (*i).second;
		_lstWeapon->addRow(2, ss1.str().c_str(), ss2.str().c_str());
	}

	for (std::map<std::string, int>::const_iterator i = _UFOTotals.begin() ; i != _UFOTotals.end() ; ++i)
	{
		if ((*i).first == "NO_UFO") continue;
		std::wstringstream ss1, ss2;

		ss1 << tr((*i).first.c_str());
		ss2 << (*i).second;
		_lstUFO->addRow(2, ss1.str().c_str(), ss2.str().c_str());
	}

}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void SoldierDiaryTotalsState::btnOkClick(Action *)
{
	_soldierDiaryState->setSoldierId(_soldier);
	_game->popState();
}

/**
 * Goes to the previous soldier.
 * @param action Pointer to an action.
 */
void SoldierDiaryTotalsState::btnPrevClick(Action *)
{
	if (_soldier == 0)
		_soldier = _base->getSoldiers()->size() - 1;
	else
		_soldier--;
	init();
}

/**
 * Goes to the next soldier.
 * @param action Pointer to an action.
 */
void SoldierDiaryTotalsState::btnNextClick(Action *)
{
	_soldier++;
	if (_soldier >= _base->getSoldiers()->size())
		_soldier = 0;
	init();
}

}