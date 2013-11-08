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
#include "SoldierDiary.h"
#include "GameTime.h"

namespace OpenXcom
{

/**
 * Initializes a new diary entry from YAML.
 * @param node YAML node.
 */
SoldierDiary::SoldierDiary(const YAML::Node &node)
{
	load(node);
}

/**
 * Constructor
 */
SoldierDiary::SoldierDiary() : _diaryEntries()
{
}

/**
 * Deconstructor
 */
SoldierDiary::~SoldierDiary()
{
	for (std::vector<SoldierDiaryEntries*>::iterator i = _diaryEntries.begin(); i != _diaryEntries.end(); ++i)
	{
		delete *i;
	}
}


/**
 * Load function
 */
void SoldierDiary::load(const YAML::Node& node)
{
	if (const YAML::Node &diaryEntries = node["diaryEntries"])
	{
		for (YAML::const_iterator i = diaryEntries.begin(); i != diaryEntries.end(); ++i)
			_diaryEntries.push_back(new SoldierDiaryEntries(*i));
	}
}

/**
 * Save function
 */
YAML::Node SoldierDiary::save() const
{
	YAML::Node node;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin(); i != _diaryEntries.end(); ++i)
			node["diaryEntries"].push_back((*i)->save());
	return node;
}

/**
 * Get the diary entries
 */
std::vector<SoldierDiaryEntries*> SoldierDiary::getSoldierDiaryEntries()
{
	return _diaryEntries;
}

/**
 *  Add soldier diary entry.
 */
void SoldierDiary::addSoldierDiaryEntry(GameTime missionTime, std::string missionRegion, std::string missionCountry, std::string missionType, std::string missionUFO, std::vector<SoldierDiaryKills*> missionKills, bool success, int rating, std::string score, std::string alienRace, int missionDaylight)
{
	_diaryEntries.push_back(new SoldierDiaryEntries(missionTime, missionRegion, missionCountry, missionType, missionUFO, missionKills, success, rating, score, alienRace, missionDaylight));
}

/**
 *
 */
std::map<std::string, int> SoldierDiary::getAlienRankTotal() const
{
	std::map<std::string, int> _list;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		std::vector<SoldierDiaryKills*> _killList = (*i)->getMissionKills();
		for (std::vector<SoldierDiaryKills*>::const_iterator j = _killList.begin() ; j != _killList.end() ; ++j)
		{
			_list[(*j)->getAlienRank().c_str()]++;
		}
	}
	return _list;
}

/**
 *
 */
std::map<std::string, int> SoldierDiary::getAlienRaceTotal() const
{
	std::map<std::string, int> _list;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		std::vector<SoldierDiaryKills*> _killList = (*i)->getMissionKills();
		for (std::vector<SoldierDiaryKills*>::iterator j = _killList.begin() ; j != _killList.end() ; ++j)
		{
			_list[(*j)->getAlienRace().c_str()]++;
		}
	}
	return _list;
}

/**
 *
 */
std::map<std::string, int> SoldierDiary::getWeaponTotal() const
{
	std::map<std::string, int> _list;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		std::vector<SoldierDiaryKills*> _killList = (*i)->getMissionKills();
		for (std::vector<SoldierDiaryKills*>::const_iterator j = _killList.begin() ; j != _killList.end() ; ++j)
		{
			_list[(*j)->getWeapon().c_str()]++;
		}
	}
	return _list;
}

/**
 *
 */
std::map<std::string, int> SoldierDiary::getWeaponAmmoTotal() const
{
	std::map<std::string, int> _list;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		std::vector<SoldierDiaryKills*> _killList = (*i)->getMissionKills();
		for (std::vector<SoldierDiaryKills*>::const_iterator j = _killList.begin() ; j != _killList.end() ; ++j)
		{
			_list[(*j)->getWeaponAmmo().c_str()]++;
		}
	}
	return _list;
}


/**
 *
 */
std::map<std::string, int> SoldierDiary::getRegionTotal() const
{
	std::map<std::string, int> _list;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		_list[(*i)->getMissionRegion().c_str()]++;
	}
	return _list;
}

/**
 *
 */
std::map<std::string, int> SoldierDiary::getCountryTotal() const
{
	std::map<std::string, int> _list;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		_list[(*i)->getMissionCountry().c_str()]++;
	}
	return _list;
}

/**
 *
 */
std::map<std::string, int> SoldierDiary::getTypeTotal() const
{
	std::map<std::string, int> _list;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		_list[(*i)->getMissionType().c_str()]++;
	}
	return _list;
}

/**
 *
 */
std::map<std::string, int> SoldierDiary::getUFOTotal() const
{
	std::map<std::string, int> _list;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		_list[(*i)->getMissionUFO().c_str()]++;
	}
	return _list;
}

/**
 *
 */
int SoldierDiary::getScoreTotal() const
{
	int _scoreTotal;
	for (std::vector<SoldierDiaryEntries*>::const_iterator i = _diaryEntries.begin() ; i != _diaryEntries.end() ; ++i)
	{
		_scoreTotal += (*i)->getMissionScore();
	}
	return _scoreTotal;
}

/**
 * Initializes a new diary entry from YAML.
 * @param node YAML node.
 */
SoldierDiaryEntries::SoldierDiaryEntries(const YAML::Node &node) : _missionTime(0,0,0,0,0,0,0)
{
	load(node);
}

/**
 * Initializes a soldier diary.
 * @param missionTime Mission's Time.
 * @param missionRegion Mission's region.
 * @param missionCountry Mission's country.
 * @param missionType Mission's type.
 * @param missionUFO Mission's UFO.
 */
SoldierDiaryEntries::SoldierDiaryEntries(GameTime missionTime, std::string missionRegion, std::string missionCountry, std::string missionType, std::string missionUFO, std::vector<SoldierDiaryKills*> missionKills, bool success, int score, std::string rating, std::string alienRace, int missionDaylight) : _missionTime(missionTime), _missionRegion(missionRegion), _missionCountry(missionCountry), _missionType(missionType), _missionUFO(missionUFO), _missionKills(missionKills), _success(success), _score(score), _rating(rating), _alienRace(alienRace), _missionDaylight(missionDaylight)
{
}

/**
 *
 */
SoldierDiaryEntries::~SoldierDiaryEntries()
{
}

/**
 * Loads the soldier-equipment layout item from a YAML file.
 * @param node YAML node.
 */
void SoldierDiaryEntries::load(const YAML::Node &node)
{
	_missionTime.load(node["missionTime"]);
	_missionRegion = node["missionRegion"].as<std::string>(_missionRegion);
	_missionCountry = node["missionCountry"].as<std::string>(_missionCountry);
	_missionType = node["missionType"].as<std::string>(_missionType);
	_missionUFO = node["missionUFO"].as<std::string>(_missionUFO);
	if (const YAML::Node &missionKills = node["missionKills"])
	{
		for (YAML::const_iterator i = missionKills.begin() ; i != missionKills.end() ; ++i)
			_missionKills.push_back(new SoldierDiaryKills(*i));
	}
	_success = node["success"].as<bool>(_success);
	_score = node["score"].as<int>(_score);
	_rating = node["rating"].as<std::string>(_rating);
	_alienRace = node["alienRace"].as<std::string>(_alienRace);
	_missionDaylight = node["missionDayligt"].as<int>(_missionDaylight);
}

/**
 * Saves the soldier-equipment layout item to a YAML file.
 * @return YAML node.
 */
YAML::Node SoldierDiaryEntries::save() const
{
	YAML::Node node;
	node["missionTime"] = _missionTime.save();
	node["missionRegion"] = _missionRegion;
	node["missionCountry"] = _missionCountry;
	node["missionType"] = _missionType;
	node["missionUFO"] = _missionUFO;
	if (!_missionKills.empty())
	{
		for (std::vector<SoldierDiaryKills*>::const_iterator i = _missionKills.begin() ; i != _missionKills.end() ; ++i)
		{
			node["missionKills"].push_back((*i)->save());
		}
	}
	node["success"] = _success;
	node["score"] = _score;
	node["rating"] = _rating;
	node["alienRace"] = _alienRace;
	node["missionDayligt"] = _missionDaylight;
	return node;
}

/**
 * Get mission time
 * @return Mission Time.
 */
const GameTime *SoldierDiaryEntries::getMissionTime() const
{
	return &_missionTime;
}

/**
 *
 */
std::string SoldierDiaryEntries::getMissionRegion() const
{
	return _missionRegion;
}

/**
 *
 */
std::string SoldierDiaryEntries::getMissionCountry() const
{
	return _missionCountry;
}

/**
 *
 */
std::string SoldierDiaryEntries::getMissionType() const
{
	return _missionType;
}

/**
 *
 */
std::string SoldierDiaryEntries::getMissionUFO() const
{
	return _missionUFO;
}

/**
 *
 */
std::vector<SoldierDiaryKills*> SoldierDiaryEntries::getMissionKills() const
{
	return _missionKills;
}

/**
 *
 */
bool SoldierDiaryEntries::getMissionSuccess() const
{
	return _success;
}

/**
 *
 */
std::string SoldierDiaryEntries::getMissionRating() const
{
	return _rating;
}

/**
 *
 */
int SoldierDiaryEntries::getMissionScore() const
{
	return _score;
}

/**
 *
 */
void SoldierDiaryEntries::addMissionKill(std::string alienRank, std::string alienRace, std::string weapon, std::string weaponAmmo, AlienState alienState)
{
	_missionKills.push_back(new SoldierDiaryKills(alienRank, alienRace, weapon, weaponAmmo, alienState));
}

/**
 *
 */
int SoldierDiaryEntries::getMissionKillTotal() const
{
	int _total = 0;
	for (std::vector<SoldierDiaryKills*>::const_iterator i = _missionKills.begin() ; i != _missionKills.end() ; ++i)
	{
		if ((*i)->getAlienState() == KILLED)
		{
			_total++;
		}
	}
	return _total;
}

/**
 *
 */
std::string SoldierDiaryEntries::getMissionRace() const
{
	return _alienRace;
}

/**
 *
 */
int SoldierDiaryEntries::getMissionDaylight() const
{
	return _missionDaylight;
}

/**
 *
 */
int SoldierDiaryEntries::getMissionStunTotal() const
{
	int _total = 0;
	for (std::vector<SoldierDiaryKills*>::const_iterator i = _missionKills.begin() ; i != _missionKills.end() ; ++i)
	{
		if ((*i)->getAlienState() == STUNNED)
		{
			_total++;
		}
	}
	return _total;
}

/**
 * Initializes a new diary entry from YAML.
 * @param node YAML node.
 */
SoldierDiaryKills::SoldierDiaryKills(const YAML::Node &node)
{
	load(node);
}

/**
 * Initializes a soldier diary.
 */
SoldierDiaryKills::SoldierDiaryKills(std::string alienRank, std::string alienRace, std::string weapon, std::string weaponAmmo, AlienState alienState) : _alienRank(alienRank), _alienRace(alienRace), _weapon(weapon), _weaponAmmo(weaponAmmo), _alienState(alienState)
{
}

/**
 *
 */
SoldierDiaryKills::~SoldierDiaryKills()
{
}

/**
 * Loads the soldier-equipment layout item from a YAML file.
 * @param node YAML node.
 */
void SoldierDiaryKills::load(const YAML::Node &node)
{
	_alienRank = node["alienRank"].as<std::string>(_alienRank);
	_alienRace = node["alienRace"].as<std::string>(_alienRace);
	_weapon = node["weapon"].as<std::string>(_weapon);
	_weaponAmmo = node["weaponAmmo"].as<std::string>(_weaponAmmo);
	_alienState = (AlienState)node["alienState"].as<int>();

}

/**
 * Saves the soldier-equipment layout item to a YAML file.
 * @return YAML node.
 */
YAML::Node SoldierDiaryKills::save() const
{
	YAML::Node node;
	node["alienRank"] = _alienRank;
	node["alienRace"] = _alienRace;
	node["weapon"] = _weapon;
	node["weaponAmmo"] = _weaponAmmo;
	node["alienState"] = (int)_alienState;
	return node;
}

/**
 *
 */
std::string SoldierDiaryKills::getAlienRank() const
{
	return _alienRank;
}

/**
 *
 */
std::string SoldierDiaryKills::getAlienRace() const
{
	return _alienRace;
}

/**
 *
 */
std::string SoldierDiaryKills::getWeapon() const
{
	return _weapon;
}

/**
 *
 */
std::string SoldierDiaryKills::getWeaponAmmo() const
{
	return _weaponAmmo;
}

/**
 *
 */
AlienState SoldierDiaryKills::getAlienState() const
{
	return _alienState;
}

}
