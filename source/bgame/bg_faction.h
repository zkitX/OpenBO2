#pragma once

enum team_t
{
	TEAM_FREE = 0x0,
	TEAM_BAD = 0x0,
	TEAM_ALLIES = 0x1,
	TEAM_AXIS = 0x2,
	TEAM_THREE = 0x3,
	TEAM_FOUR = 0x4,
	TEAM_FIVE = 0x5,
	TEAM_SIX = 0x6,
	TEAM_SEVEN = 0x7,
	TEAM_EIGHT = 0x8,
	TEAM_NUM_PLAYING_TEAMS = 0x9,
	TEAM_SPECTATOR = 0x9,
	TEAM_NUM_TEAMS = 0xA,
	TEAM_LOCALPLAYERS = 0xB,
	TEAM_FIRST_PLAYING_TEAM = 0x1,
	TEAM_LAST_PLAYING_TEAM = 0x8,
};

class bg_faction
{
};

